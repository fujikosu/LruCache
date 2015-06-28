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
	//LRU�L���b�V���{��
	CacheList mCacheList;
	//�f�[�^�ւ̃A�N�Z�X��O(1)�ŒS��
	CacheMap mCacheMap;
public:
	LRUCache(size_t size){
		//�����10�Œ�
		capacity = 10;
		entries = 0;
	}
	//�L���b�V���ւ̑}��
	void insert(std::string key, int data){
		//�V�f�[�^�̏ꍇ
		if (mCacheMap.count(key) == 0)
		{
			//list�̐擪�ɃN�G���ǉ�
			mCacheList.push_front(std::make_pair(key, data));
			//�|�C���^�̈ʒu��ێ�
			mCacheMap[key] = mCacheList.begin();
			entries++;
			if (entries > capacity)
			{
				//key�����list�̍Ō���������|�C���^������
				mCacheMap.erase(mCacheList.back().first);
				//�L���b�V����LRU�v�f���폜
				mCacheList.pop_back();
				entries--;
			}
		}
		//���ɂ���f�[�^�̏ꍇ
		else
		{
			mCacheList.erase(mCacheMap[key]);
			mCacheList.push_front(std::make_pair(key, data));
			//�|�C���^�̈ʒu��擪�ɍX�V
			mCacheMap[key] = mCacheList.begin();
		}
	}
	//key����ɒl�̎擾
	int get(std::string key){
		if (mCacheMap.count(key) == 1){
			EntryPair value = *mCacheMap[key];
			//���݂̈ʒu����폜���A�ŏ��̈ʒu�ɕύX
			mCacheList.erase(mCacheMap[key]);
			mCacheList.push_front(value);
			//�|�C���^�̈ʒu��擪�ɍX�V
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