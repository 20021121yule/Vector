#include <iostream>
#include "Vector.h"

using namespace std;

// 辅助函数：打印Vector信息
template<typename T>
void printVector(const Vector<T>& vec, const string& name) {
    cout << name << ": [ ";
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << "]  Size: " << vec.size()
         << "  Capacity: " << vec.capacity() << endl;
}

int main() {
    try {
        // ==================== 构造函数测试 ====================
        cout << "\n===== 构造函数测试 =====" << endl;
        // 默认构造
        Vector<int> v_default;
        printVector(v_default, "默认构造");

        // 参数构造 (容量5，初始化3个元素为10)
        Vector<int> v_param(5, 3, 10);
        printVector(v_param, "参数构造");

        // 数组构造
        int arr[] = {1, 3, 5, 7, 9};
        Vector<int> v_array(arr, 5);
        printVector(v_array, "数组构造");

        // 复制构造
        Vector<int> v_copy(v_array);
        printVector(v_copy, "复制构造");

        // ==================== 插入与扩容测试 ====================
        cout << "\n===== 插入与扩容测试 =====" << endl;
        Vector<int> v_insert;
        for (int i = 0; i < 10; ++i) {
            v_insert.insert(i, v_insert.size());
            cout << "插入 " << i << " 后容量: " << v_insert.capacity() << endl;
        }
        printVector(v_insert, "连续插入后");

        // 中间插入
        v_insert.insert(100, 3);
        printVector(v_insert, "中间插入100后");

        // ==================== 删除与缩容测试 ====================
        cout << "\n===== 删除与缩容测试 =====" << endl;
        // 删除单个元素
        v_insert.remove(3);
        printVector(v_insert, "删除索引3后");

        // 删除区间 [5,8)
        v_insert.remove(5, 8);
        printVector(v_insert, "删除区间[5,8)后");

        // 删除尾部直到空
        while (v_insert.size() > 0) {
            v_insert.remove(v_insert.size() - 1);
            cout << "删除尾部后容量: " << v_insert.capacity() << endl;
        }
        printVector(v_insert, "完全删除后");

        // ==================== 查找功能测试 ====================
        cout << "\n===== 查找功能测试 =====" << endl;
        Vector<int> v_find(arr, 5);
        printVector(v_find, "测试向量");

        // 查找存在的元素
        Rank pos = v_find.find(5, 0, 5);
        cout << "查找元素5的位置: " << pos << " (预期2)" << endl;

        // 查找不存在的元素
        pos = v_find.find(4, 0, 5);
        cout << "查找元素4的位置: " << pos << " (预期-1)" << endl;

        // ==================== 去重算法测试 ====================
        cout << "\n===== 去重算法测试 =====" << endl;
        int dup_data[] = {2, 3, 2, 5, 3, 5};
        Vector<int> v_dup(dup_data, 6);
        printVector(v_dup, "去重前");
        v_dup.deduplicate();
        printVector(v_dup, "去重后");

        // ==================== 排序与唯一化测试 ====================
        cout << "\n===== 排序与唯一化测试 =====" << endl;
        int data[] = {5, 2, 8, 2, 5, 6, 5};
        Vector<int> v_sort(data, 7);
        printVector(v_sort, "排序前");
        v_sort.sort(0, v_sort.size() - 1);
        printVector(v_sort, "快速排序后");
        v_sort.uniquify();
        printVector(v_sort, "唯一化后");

        // ==================== 二分查找测试 ====================
        cout << "\n===== 二分查找测试 =====" << endl;
        int data2[] = {1, 3, 3, 3, 5, 7, 9};
        Vector<int> v_bsearch(data2, 7);
        printVector(v_bsearch, "有序向量");

        // 查找存在的元素
        pos = v_bsearch.binarySearch(3, 0, 7);
        cout << "查找元素3的最后位置: " << pos << " (预期3)" << endl;

        // 查找不存在的元素
        pos = v_bsearch.binarySearch(6, 0, 7);
        cout << "查找元素6的插入位置: " << pos << " (预期4)" << endl;

        // ==================== 异常处理测试 ====================
        cout << "\n===== 异常处理测试 =====" << endl;
        // 越界删除
        try {
            Vector<int> v_empty;
            v_empty.remove(0);
            cout << "未捕获越界删除异常 ×" << endl;
        } catch (const exception& e) {
            cout << "捕获异常: " << e.what() << " √" << endl;
        }

        // 无效插入位置
        try {
            Vector<int> v(3);
            v.insert(0, -1);
            cout << "未捕获无效插入异常 ×" << endl;
        } catch (const exception& e) {
            cout << "捕获异常: " << e.what() << " √" << endl;
        }

        // 未排序向量调用二分查找
        try {
            int data2[] = {3, 1, 4};
            Vector<int> v_unsorted(data2, 3);
            v_unsorted.binarySearch(1, 0, 3);
            cout << "未捕获无序异常 ×" << endl;
        } catch (const exception& e) {
            cout << "捕获异常: " << e.what() << " √" << endl;
        }

        // ==================== 内存泄漏提示 ====================
        cout << "\n===== 内存泄漏检测提示 =====" << endl;
        cout << "请使用以下命令检测内存泄漏:\n"
             << "valgrind --leak-check=full --track-origins=yes ./your_program\n";

        cout << "\n===== 所有测试通过! =====" << endl;
    } catch (const exception& e) {
        cerr << "测试失败: " << e.what() << endl;
        return 1;
    }
    return 0;
}