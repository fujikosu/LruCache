//#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <opencv2/highgui/highgui.hpp>


template<typename DATA>
class LRUCache{
    typedef std::pair<std::string, DATA> EntryPair;
    typedef std::list<EntryPair> CacheList;
    typedef std::unordered_map<std::string, typename CacheList::iterator> CacheMap;
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
        //std::cout << "insert " << data << std::endl; 
        //新データの場合（挿入しサイズをオーバーする場合は一番古いものを削除）
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
        //既にあるデータの場合（位置を一番前にもってくるだけ）
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
    DATA get(std::string key){
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
        //キャッシュに求める値が入っていない場合　（エラーを投げる）
        else
        {
            throw "NOT IN YOUR CACHE";
            //値を返すようにする場合はこちら
            //if (typeid(DATA) == typeid(cv::Mat))
            //{
            //  cv::Mat mtx(cv::Size(100,100), CV_16U,cv::Scalar(0,0,0));
            //  return mtx;
            //}
            //if (typeid(DATA) == typeid(int))
            //{
            //  return -1;
            //}
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

//コメントアウト部をonにすればキャッシュにない画像にアクセスすることになるためエラーを吐くようになっている
void imageTest(LRUCache<cv::Mat>& lruCache){
    std::string filepath1 = "Sample Pictures\\angkor-wat.jpg";
    std::string filepath2 = "Sample Pictures\\castle.jpg";
    std::string filepath3 = "Sample Pictures\\Chrysanthemum.jpg";
    std::string filepath4 = "Sample Pictures\\Desert.jpg";
    std::string filepath5 = "Sample Pictures\\Hydrangeas.jpg";
    std::string filepath6 = "Sample Pictures\\Jellyfish.jpg";
    std::string filepath7 = "Sample Pictures\\Koala.jpg";
    std::string filepath8 = "Sample Pictures\\Lake.jpg";
    std::string filepath9 = "Sample Pictures\\Lighthouse.jpg";
    std::string filepath10 = "Sample Pictures\\Penguins.jpg";
    std::string filepath11 = "Sample Pictures\\Statue.jpg";
    std::string filepath12 = "Sample Pictures\\Tulips.jpg";
    
    cv::Mat img1 = cv::imread(filepath1,1);
    cv::Mat img2 = cv::imread(filepath2,1);
    cv::Mat img3 = cv::imread(filepath3,1);
    cv::Mat img4 = cv::imread(filepath4,1);
    cv::Mat img5 = cv::imread(filepath5,1);
    cv::Mat img6 = cv::imread(filepath6,1);
    cv::Mat img7 = cv::imread(filepath7,1);
    cv::Mat img8 = cv::imread(filepath8,1);
    cv::Mat img9 = cv::imread(filepath9,1);
    cv::Mat img10 = cv::imread(filepath10,1);
    cv::Mat img11 = cv::imread(filepath11,1);
    cv::Mat img12 = cv::imread(filepath12,1);
    
    lruCache.insert(filepath1,img1);
    lruCache.insert(filepath2,img2);
    lruCache.insert(filepath3,img3);
    lruCache.insert(filepath4,img4);
    lruCache.insert(filepath5,img5);
    lruCache.insert(filepath6,img6);
    lruCache.insert(filepath7,img7);
    lruCache.insert(filepath8,img8);
    lruCache.insert(filepath9,img9);
    lruCache.insert(filepath10,img10);

    cv::namedWindow("image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("image1",lruCache.get(filepath1));
    cv::waitKey(0);
    
    //cv::namedWindow("image2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    //cv::imshow("image2",lruCache.get(filepath11));
    //cv::waitKey(0);
    
    lruCache.insert(filepath11,img11);
    cv::namedWindow("image3", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("image3",lruCache.get(filepath11));
    cv::waitKey(0);
    
    lruCache.insert(filepath12,img12);
    //cv::namedWindow("image4", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
 //   cv::imshow("image4",lruCache.get(filepath2));
    //cv::waitKey(0);
    
    cv::namedWindow("image5", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("image5",lruCache.get(filepath12));
    cv::waitKey(0);
    
    return;
}


int main(){
    //こちらを使えばint用のキャッシュが使えるがキャッシュ内に値が入っていない場合はエラーが出て止まるようになっている
    //LRUCache<int> lruCache(10);
    //std::thread thread1(test,std::ref(lruCache));
    //std::thread thread2(test2,std::ref(lruCache));
    //thread1.join();
    //thread2.join();
    //std::cout << "check " << 8 << ":" << lruCache.get("eight") << std::endl;

    //画像キャッシュテスト用
    LRUCache<cv::Mat> lruCacheImg(10);
    std::thread thread3(imageTest,std::ref(lruCacheImg));
    thread3.join();
}
