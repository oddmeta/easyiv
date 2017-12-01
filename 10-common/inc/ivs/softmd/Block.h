#ifndef __BLOCK__H
#define __BLOCK__H

#include <iostream>

class Block{
//========friend=======
	friend class BlockWarper;
	friend std::ostream& operator<<(std::ostream& ooo, const Block& block);
private:
	int x, y, w, h, sens;
	int refCnt;    //: for smart point
	int diffs;
	bool isDect;
private:
	Block() { isDect = true; } 
public:
	static int instanceCnt;
	Block(int x, int y, int w, int h) 
		: x(x), y(y), w(w), h(h), sens(0), refCnt(0) { }
	Block(int x, int y, int w, int h, int sens)
		: x(x), y(y), w(w), h(h), sens(sens), refCnt(0) { }
	inline bool operator==(const Block& blk) const {
		return (   x==blk.x
				&& y==blk.y
				&& w==blk.w
				&& h==blk.h
			   );
	}
	inline void setNeedDect(bool d){ isDect = d; }
	inline bool isNeedDect() const { return isDect; }
	inline int getDiffs() const { return diffs; }
    void* operator new(size_t s){
		++instanceCnt;
		return ::operator new(s);
    }
    void operator delete(void* p){
		--instanceCnt;
		::operator delete(p);
    }

};

class BlockWarper{
	friend std::ostream& operator<<(std::ostream& ooo, const BlockWarper& block);
private:
	Block*content;
private:
	BlockWarper() { } 
	int testing(int*imageArray, int w, int h) const;
public:
	BlockWarper(const Block& b) 
		: content(new Block(b.x, b.y, b.w, b.h, b.sens)){ }
	BlockWarper(const BlockWarper& bw) 
		: content(bw.content) { ++content->refCnt; }
	BlockWarper& operator=(const BlockWarper& bw);
	
	void setSenstive(int sens) { content->sens = sens; }

	inline bool operator==(const BlockWarper& blkw) const { 
		return *content == *(blkw.content); 
	}
	inline int getX() const { return this->content->x; }
	inline int getY() const { return this->content->y; }
	inline int getW() const { return this->content->w; }
	inline int getH() const { return this->content->h; }
	inline int getSens() const { return this->content->sens; }    
    inline int getDiffs() const { return this->content->getDiffs(); }
	inline void setNeedDect(bool dect) { content->setNeedDect(dect); }
	inline bool isNeedDect() const { return content->isNeedDect(); }

	bool dectM(int*imageArray, int w, int h) const;
	bool inBlock(int x, int y) const;
	~BlockWarper();
};

#endif// __BLOCK__H