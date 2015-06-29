//#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <pthread.h>
#include <thread>
#include <mutex>

typedef std::pair<std::string, int> EntryPair;
typedef std::list<EntryPair> CacheList;
typedef std::unordered_map<std::string, CacheList::iterator> CacheMap;


class LRUCache{
private:
    int capacity;
    int entries;
    //LRUキャッシュ本体
    CacheList mCacheList;
    //データへのアクセスをO(1)で担保
    CacheMap mCacheMap;

public:
    //pthread_mutex_t mutex;
    std::mutex mutex;
    LRUCache(size_t size){
        //今回は10固定
        capacity = 10;
        entries = 0;
    }
    //キャッシュへの挿入
    void insert(std::string key, int data){
        //pthread_mutex_lock(&mutex);
        mutex.lock();
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
        //pthread_mutex_unlock(&mutex);
        mutex.unlock();
    }
    //keyを基に値の取得
    int get(std::string key){
        //pthread_mutex_lock(&mutex);
        if (mCacheMap.count(key) == 1){
            mutex.lock();
            EntryPair value = *mCacheMap[key];
            //現在の位置から削除し、最初の位置に変更
            mCacheList.erase(mCacheMap[key]);
            mCacheList.push_front(value);
            //ポインタの位置を先頭に更新
            mCacheMap[key] = mCacheList.begin();
            //pthread_mutex_unlock(&mutex);
            mutex.unlock();
            return value.second;
        }
        else
        {
            //pthread_mutex_unlock(&mutex);
            return 0;
        }
    }

    void move(std::string key){

    }
};

//void* test( void* cache){
//  LRUCache* lruCache = (LRUCache*) cache;
//  lruCache->insert("one",1);
//  lruCache->insert("two",2);
//  lruCache->insert("three",3);
//  lruCache->insert("four",4);
//  lruCache->insert("five",5);
//  lruCache->insert("six",6);
//  lruCache->insert("seven",7);
//  lruCache->insert("eight",8);
//  lruCache->insert("nine",9);
//  lruCache->insert("ten",10);
//  std::cout << lruCache->get("one") << std::endl;
//  std::cout << lruCache->get("eleven")<< std::endl;
//  lruCache->insert("eleven",11);
//  std::cout << lruCache->get("two")<< std::endl;
//  std::cout << lruCache->get("eleven")<< std::endl;
//  lruCache->insert("twelve",12);
//  std::cout << lruCache->get("twelve")<< std::endl;
//  std::cout << lruCache->get("three")<< std::endl;
//  std::cout << lruCache->get("thirteen")<< std::endl;
//  //pthread_exit(NULL);
//  return 0;
//}

void test(LRUCache* lruCache){
    lruCache->insert("one",1);
    lruCache->insert("two",2);
    lruCache->insert("three",3);
    lruCache->insert("four",4);
    lruCache->insert("five",5);
    lruCache->insert("six",6);
    lruCache->insert("seven",7);
    lruCache->insert("eight",8);
    lruCache->insert("nine",9);
    lruCache->insert("ten",10);
    std::cout << lruCache->get("one") << std::endl;
    std::cout << lruCache->get("eleven")<< std::endl;
    lruCache->insert("eleven",11);
    std::cout << lruCache->get("two")<< std::endl;
    std::cout << lruCache->get("eleven")<< std::endl;
    lruCache->insert("twelve",12);
    std::cout << lruCache->get("twelve")<< std::endl;
    std::cout << lruCache->get("three")<< std::endl;
    std::cout << lruCache->get("thirteen")<< std::endl;
    //pthread_exit(NULL);
    return;
}

int main(){
    pthread_t th1,th2;
    LRUCache* lruCache = new LRUCache(10);
    std::thread thread1(test,lruCache);
    std::thread thread2(test,lruCache);
    thread1.join();
    thread2.join();
    //pthread_mutex_init(&lruCache->mutex, NULL);
    //pthread_create(&th1,NULL,test,&lruCache);
    //pthread_create(&th2,NULL,test,&lruCache);
    //pthread_join(th1,NULL);
    //pthread_join(th2,NULL);
    //test(lruCache);
    //pthread_mutex_destroy(&lruCache->mutex);
}