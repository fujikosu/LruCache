#include "stdafx.h"
#include <list>
#include <hash_map>

typedef std::pair<std::string, int> EntryPair;
typedef std::list<EntryPair> CacheList;
typedef stdext::hash_map<std::string, CacheList::iterator> CacheMap;


class LRUCache{
private:
	int capacity;
	int entries;
	//LRUキャッシュ本体
	CacheList mCacheList;
	//データへのアクセスをO(1)で担保
	CacheMap mCacheMap;
public:
	LRUCache(size_t size){
		//今回は10固定
		capacity = 10;
		entries = 0;
	}
	//キャッシュへの挿入
	void insert(std::string key, int data){
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
	}
	//keyを基に値の取得
	int get(std::string key){
		if (mCacheMap.count(key) == 1){
			EntryPair value = *mCacheMap[key];
			//現在の位置から削除し、最初の位置に変更
			mCacheList.erase(mCacheMap[key]);
			mCacheList.push_front(value);
			//ポインタの位置を先頭に更新
			mCacheMap[key] = mCacheList.begin();
			return value.second;
		}
		else
		{
			return 0;
		}
	}

	void move(std::string key){

	}
};



int main(){

}