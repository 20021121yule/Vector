//
// Created by 余乐 on 25-3-5.
//

#ifndef VECTOR_H
#define VECTOR_H

typedef int Rank;
#define DEFAULT_CAPACITY 3

#include <random>

template<typename T>
class Vector {
protected:
    // 内置参量
    Rank _size{};
    int _capacity{};
    T *_elem;

    // 内置函数
    void copyFrom(const T *other, Rank lo, Rank hi); // 复制函数
    void expand();

    void shrink();

    void permute(Vector<T> &vec);

    // 判等器, 判断两（个）组Vector是否具有相同的元素
    static bool eq(T &a, T &b); // 直接在元素之间比较
    static bool eq(const Vector<T> &a, const Vector<T> &b); // 向量类之间的比较

    // 有序性判断器，返回值为0，那么完全有序（规定按照从小到大排序），如果非0，那么存在逆序对，不完全有序
    [[nodiscard]] int disorder() const;

public:
    // 构造函数
    explicit Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        // c是容量,默认为3，s是规模，默认为0，s<=c
        // v是对所有元素的初始化，都初始化为一样的值，默认为0
        this->_elem = new T[this->_capacity = c]; // 这里是提前为_elem创建那么大容量的内存
        // 注意elem的初始化方式，这里显示_elem的实质上是一个数组

        for (this->_size = 0; this->_size < s;) {
            this->_elem[this->_size++] = v;
        } // 这里同时也初始化了_size
    }

    Vector(T *&other, Rank lo, Rank hi) { copyFrom(other, lo, hi); } // 输入是一个T类型的数组指针，例如{1,2,3}
    Vector(T *&other, Rank n) { copyFrom(other, 0, n); }
    Vector(const T other[], Rank n) { copyFrom(other, 0, n); }
    Vector(const Vector<T> &other, Rank lo, Rank hi) { copyFrom(other._elem, lo, hi); } // 输入是一个T类型的Vector
    Vector(const Vector<T> &other) { copyFrom(other._elem, 0, other._size); }

    // 析构函数
    ~Vector() { delete[] this->_elem; }

    // 只读访问接口
    [[nodiscard]] int size() const { return this->_size; } // 返回大小属性函数
    [[nodiscard]] int capacity() const { return this->_capacity; } // 返回容量属性函数
    [[nodiscard]] bool empty() const { return this->_size == 0; } // 判断是否是空集
    Rank find(const T &e) const { return find(e, 0, this->_size); } // 整体查找(无视排序)
    Rank find(const T &e, Rank lo, Rank hi) const; // 查找(无视排序)
    Rank research(const T &e) const { return (0 >= this->_size) ? -1 : binarySearch(e, 0, this->_size); }
    Rank binarySearch(const T &e, Rank lo, Rank hi) const; // 二分查找函数（从大到小排序后）

    // 可写访问接口
    T &operator[](Rank i) const;
    Vector<T> &operator=(const Vector<T> &other); //重载操作符
    void insert(const T &e, Rank n); // 插入, n为插入下下标
    void insert(const T &e) { insert(e, this->_size); } // 默认为在最后位置插入
    void remove(Rank n); // 删除，删除Rank = n的元素
    void remove(Rank lo, Rank hi); // 删除，删除[lo, hi)区间中的元素
    void unsort(Rank lo, Rank hi); //置乱器,等概率置乱区间[lo,hi)
    void sort(Rank lo, Rank hi); // 排序算法(快速排序)
    void sort() { sort(0, this->_size - 1); } // 默认是全部排序
    void deduplicate(); // 置乱器：对于未排序或已排序的Vector可以用
    void uniquify(); // 置乱器：对于已经排序的Vector使用，时间复杂度比上一个小很多
};

// 模版成员函数的实现必须在头文件中

template<typename T>
void Vector<T>::copyFrom(const T *other_elem, Rank lo, Rank hi) {
    // 以other的[lo, hi)为蓝本复制变量
    this->_elem = new T[this->_capacity = 2 * (hi - lo)]; // 注意这里把_elem和_capacity一并初始化了
    this->_size = 0; // 这里放到下面while循环一起来累加

    // 复制实现
    while (lo < hi) {
        this->_elem[_size++] = other_elem[lo++];
    }
}

template<typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    // 处理自赋值
    if (this == &other) {
        return *this;
    }

    // 先删除原有的数据
    if (this->_elem) {
        delete[] this->_elem;
    }

    // 将元素都复制到_elem当中，当然_capacity和_size都会重新计算
    copyFrom(other._elem, 0, other._size);

    return *this;
}

template<typename T>
T &Vector<T>::operator[](Rank i) const {
    return this->_elem[i];
}

template<typename T>
void Vector<T>::expand() {
    if (this->_size < this->_capacity) { return; } // 未满员
    if (this->_capacity < DEFAULT_CAPACITY) { this->_capacity = DEFAULT_CAPACITY; } // 把容量提升到默认容量

    T *old_elem = this->_elem;
    this->_elem = new T[this->_capacity <<= 1]; // 容量翻倍

    for (int i = 0; i < _size; i++) {
        this->_elem[i] = old_elem[i];
    }

    delete[] old_elem;
}

template<typename T>
void Vector<T>::shrink() {
    if (this->_capacity < DEFAULT_CAPACITY << 1) { return; }
    if (this->_size << 2 > this->_capacity) { return; } // 以25%为界,实际占用等于容量25%时就执行shrink操作

    T *old_elem = this->_elem;
    this->_elem = new T[this->_capacity >>= 1]; // 缩小一半

    for (int i = 0; i < _size; i++) {
        this->_elem[i] = old_elem[i];
    }

    delete[] old_elem;
}

template<typename T>
void Vector<T>::permute(Vector<T> &vec) {
    // 使用静态随机数引擎，避免重复初始化
    static std::random_device rd; // 随机设备用于种子
    static std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎

    for (int i = vec._size; i > 0; --i) {
        // 修正 vec._size() → vec._size
        // 生成 [0, i-1] 范围内的均匀分布整数
        std::uniform_int_distribution<int> dis(0, i - 1);
        int j = dis(gen);
        std::swap(vec[i - 1], vec[j]);
    }
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    // 参数校验
    if (lo < 0 || hi > this->_size || lo >= hi) {
        throw std::invalid_argument("Invalid range for unsort");
    }

    // 使用更高质量的随机数生成器
    static std::random_device rd;
    static std::mt19937 gen(rd());

    T *ptr = this->_elem + lo;
    const int n = hi - lo;

    // 正确的Fisher-Yates洗牌算法
    for (int i = n - 1; i > 0; --i) {
        std::uniform_int_distribution<> dis(0, i); // 包含i的均匀分布
        int j = dis(gen);
        std::swap(ptr[i], ptr[j]);
    }
}

template<typename T>
bool Vector<T>::eq(const Vector<T> &a, const Vector<T> &b) {
    if (a.size() != b.size()) { return false; } // 大小不等直接判错

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) { return false; }
    } // 中间有一个不相等的直接判错

    return true;
}

template<typename T>
bool Vector<T>::eq(T &a, T &b) {
    return a == b;
}

template<typename T>
Rank Vector<T>::find(const T &e, Rank lo, Rank hi) const {
    if (lo < 0 || hi > _size || lo > hi) {
        throw std::out_of_range("Vector::find: Invalid range");
    }

    while ((lo < hi--) && (e != this->_elem[hi])) {
    }; // 从后往前找， hi--是先使用当前值，再自减
    return hi; // 如果返回的是lo - 1， 说明查找失败
}

template<typename T>
void Vector<T>::insert(const T &e, Rank n) {
    if (n < 0 || n > _size) {
        throw std::out_of_range("Vector::insert: Index out of range");
    }

    expand(); // 先扩容

    for (int i = _size; i > n; i--) {
        this->_elem[i] = this->_elem[i - 1];
    } // 往右侧挪一个位置

    this->_elem[n] = e; // 在规定位置插入元素

    ++this->_size;
}

template<typename T>
void Vector<T>::remove(Rank lo, Rank hi) {
    if (lo < 0 || hi > _size || lo > hi) {
        throw std::out_of_range("Vector::remove: Invalid range");
    }
    if (lo == hi) return;

    // 原有删除逻辑保持不变
    for (Rank i = hi; i < _size; i++) {
        _elem[lo + (i - hi)] = _elem[i];
    }
    _size -= (hi - lo);
    shrink();
}

template<typename T>
void Vector<T>::remove(Rank n) {
    if (n < 0 || n >= _size) {
        throw std::out_of_range("Vector::remove: Index out of range");
    }

    if (n == this->_size - 1) {
        Rank current_index = n - 1;
        this->_size = current_index;
        shrink();
    } else {
        remove(n, n + 1);
    }
}

template<typename T>
void Vector<T>::deduplicate() {
    Rank i = 1; // 不需要从0开始
    while (i < this->_size) {
        if (find(this->_elem[i], 0, i) < 0) {
            i++; // 无重复，继续检查下一个
        } else {
            remove(i); // 有重复，删除当前元素，i不递增（因为删除后下一元素已移至i位置）
        } // 三元表达式：a？b：c要求b和c的类型要一致
    }
}

template<typename T>
int Vector<T>::disorder() const {
    int count = 0;

    for (int i = 1; i < this->_size; i++) {
        if (this->_elem[i] < this->_elem[i - 1]) {
            count++;
        }
    } // 假定从小到大排序的

    return count; // 如果完全有序，返回值应该是0
}


template<typename T>
void Vector<T>::uniquify() {
    int disorder_count = this->disorder();
    if (disorder_count != 0) {
        throw std::range_error("Vector::uniquify:"
            " The elements in the Vector is not sorted. "
            "If you want to deduplicate the Vector, you should use function 'deduplicate'.'");
    }

    Rank i = 0, j = 0;

    while (++j < this->_size) {
        if (this->_elem[i] != this->_elem[j]) {
            this->_elem[++i] = this->_elem[j];
        }
    }

    this->_size = ++i;
    shrink();
}

template<typename T>
Rank Vector<T>::binarySearch(const T &e, Rank lo, Rank hi) const {
    if (const int disorder_count = this->disorder(); disorder_count != 0) {
        throw std::range_error("Vector::binarySearch:"
            " The elements in the Vector is not sorted. "
            "If you want to search a certain element in the Vector, you should use function 'find'.");
    }

    while (lo < hi) {
        Rank mid = lo + (hi - lo) / 2;
        (e < this->_elem[mid]) ? hi = mid : lo = mid + 1;
    }

    return --lo; // 不大于e元素的最大位置
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    // 参数校验：确保范围合法
    if (lo < 0 || hi >= this->_size || lo >= hi) return;

    // 分区操作
    T pivot = this->_elem[hi];
    Rank i = lo - 1;
    for (Rank j = lo; j < hi; ++j) {
        if (this->_elem[j] < pivot) {
            std::swap(this->_elem[++i], this->_elem[j]);
        }
    }
    std::swap(this->_elem[++i], this->_elem[hi]); // 恢复

    // 递归排序子区间
    sort(lo, i - 1);
    sort(i + 1, hi);
}


#endif //VECTOR_H
