//#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <thread>
#include <mutex>


typedef std::pair<std::string, int> EntryPair;
typedef std::list<EntryPair> CacheList;
typedef std::unordered_map<std::string, CacheList::iterator> CacheMap;

template<typename DATA>
class LRUCache{
private:
    int capacity;
    int entries;
    //LRUキャッシュ本体
    CacheList mCacheList;
    //データへのアクセスをO(1)で担保
    CacheMap mCacheMap;

public:
    std::mutex mutex;
    LRUCache(size_t size){
        //今回は10固定
        capacity = 10;
        entries = 0;
    }
    ~LRUCache(){
        mCacheList.clear();
        mCacheMap.clear();
    }
    //キャッシュへの挿入
    void insert(std::string key, DATA data){
        mutex.lock();
        std::cout << "insert " << data << std::endl; 
        //新データの場合
        if (mCacheMap.count(key) == 0)
        {
            //listの先頭にクエリ追加
            mCacheList.push_front(std::make_pair(key, data));
            //ポインタの位置を保持
            mCacheMap[key] = mCacheList.begin();
            entries++;
            if (entries > capacity)
            {
                //keyを基にlistの最後尾をさすポインタを消去
                mCacheMap.erase(mCacheList.back().first);
                //キャッシュのLRU要素を削除
                mCacheList.pop_back();
                entries--;
            }
        }
        //既にあるデータの場合
        else
        {
            mCacheList.erase(mCacheMap[key]);
            mCacheList.push_front(std::make_pair(key, data));
            //ポインタの位置を先頭に更新
            mCacheMap[key] = mCacheList.begin();
        }
        mutex.unlock();
    }
    //keyを基に値の取得
    int get(std::string key){
        if (mCacheMap.count(key) == 1){
            mutex.lock();
            EntryPair value = *mCacheMap[key];
            //現在の位置から削除し、最初の位置に変更
            mCacheList.erase(mCacheMap[key]);
            mCacheList.push_front(value);
            //ポインタの位置を先頭に更新
            mCacheMap[key] = mCacheList.begin();
            mutex.unlock();
            return value.second;
        }
        else
        {
            return -1;
        }
    }
};

void test(LRUCache<int>& lruCache){
    lruCache.insert("one",1);
    lruCache.insert("two",2);
    lruCache.insert("three",3);
    lruCache.insert("four",4);
    lruCache.insert("five",5);
    lruCache.insert("six",6);
    lruCache.insert("seven",7);
    lruCache.insert("eight",8);
    lruCache.insert("nine",9);
    lruCache.insert("ten",10);
    std::cout << "check " << 1 << ":" << lruCache.get("one") << std::endl;
    std::cout << "check " << 11 << ":" << lruCache.get("eleven")<< std::endl;
    lruCache.insert("eleven",11);
    std::cout << "check " << 2 << ":" << lruCache.get("two")<< std::endl;
    std::cout << "check " << 11 << ":" << lruCache.get("eleven")<< std::endl;
    lruCache.insert("twelve",12);
    std::cout << "check " << 12 << ":" << lruCache.get("twelve")<< std::endl;
    std::cout << "check " << 3 << ":" << lruCache.get("three")<< std::endl;
    std::cout << "check " << 13 << ":" << lruCache.get("thirteen")<< std::endl;
    return;
}

void test2(LRUCache<int>& lruCache){
    lruCache.insert("1one",101);
    lruCache.insert("1two",102);
    lruCache.insert("1three",103);
    lruCache.insert("1four",104);
    lruCache.insert("1five",105);
    lruCache.insert("1six",106);
    lruCache.insert("1seven",107);
    lruCache.insert("1eight",108);
    lruCache.insert("1nine",109);
    lruCache.insert("1ten",110);
    std::cout << "check " << 101 << ":" << lruCache.get("1one") << std::endl;
    std::cout << "check " << 111 << ":" << lruCache.get("1eleven")<< std::endl;
    lruCache.insert("1eleven",111);
    std::cout << "check " << 102 << ":" << lruCache.get("1two")<< std::endl;
    std::cout << "check " << 111 << ":" << lruCache.get("1eleven")<< std::endl;
    lruCache.insert("1twelve",112);
    std::cout << "check " << 112 << ":" << lruCache.get("1twelve")<< std::endl;
    std::cout << "check " << 103 << ":" << lruCache.get("1three")<< std::endl;
    std::cout << "check " << 113 << ":" << lruCache.get("1thirteen")<< std::endl;
    return;
}


int main(){
    LRUCache<int> lruCache(10);
    std::thread thread1(test,std::ref(lruCache));
    std::thread thread2(test2,std::ref(lruCache));
    thread1.join();
    thread2.join();
    std::cout << "check " << 8 << ":" << lruCache.get("eight") << std::endl;
}