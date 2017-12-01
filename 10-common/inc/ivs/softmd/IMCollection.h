#ifndef __IMCOLLECTION__H
#define __IMCOLLECTION__H

#include <list>
#include "Block.h"

class IMCollection
{
	friend std::ostream& operator<<(std::ostream& ooo, const IMCollection& im);
private:
	std::list<BlockWarper>blocks;
	int w, h;	
public:
	IMCollection getAllBlocks(int*imageArray, int w, int h);
	int defaultSens;
	IMCollection() : defaultSens(0) { }
	IMCollection(int defaultSens) : defaultSens(defaultSens) { }

	inline int count() const { return blocks.size(); }
	inline void add(int x, int y, int w, int h){ add(x, y, w, h, defaultSens);  }
	inline void add(const BlockWarper& blk) { blocks.push_back(blk); }
    inline BlockWarper& operator[](int n) { return items(n); }

	void add(int x, int y, int w, int h, int sens);	
	void remove(int n);

	void setNeedDect(int idx, bool needDect);

	IMCollection search(int x, int y) const ;  //: find all match in blocks
	BlockWarper& items(int n) ;

	IMCollection getChangedBlocks(int*imageArray, int w, int h)const;
};
#endif