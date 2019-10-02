#ifndef _fdd_h_
#define _fdd_h_

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <utilfuncs/utilfuncs.h>

typedef std::function<void(int, int, std::string)> PCBF;
auto pcbdef=[](int, int, std::string){ return; };

//===================================================================================================
using POSTYPE=size_t;
const POSTYPE INVALID_POS=POSTYPE(-1); //primarily used to indicate an entry occurring in only one file

enum DIFFSTATE
{
	DS_ERR=0, //
	DS_EQU, //lines are equal
	DS_NEQ, //(binary) lines are not equal (split data at index(default==16))
	DS_NEW, //(binary) line exist R-only (empty line at L)
	DS_RIGHT=DS_NEW,
	DS_OLD, //(binary) line exist L-only (empty line at R)
	DS_LEFT=DS_OLD,
	DS_MOV, //(text) line-content at different positions
	DS_ADD, //(text) line exist R-only
	DS_DEL, //(text) line exist L-only
};

using FPos=std::vector<POSTYPE>; //line-numbers/offsets, index-sync'ed with data
using FLine=std::vector<std::string>; //data content, index-sync'ed with line/offset above
//maybe use tuple<POSTYPE, POSTYPE, std::string> ?

//===================================================================================================
enum DIFFTYPE { DTS_UNK=0, DTS_TXT, DTS_BIN, DTS_DIR, };
struct DiffType
{
	std::string lpath{};
	std::string rpath{};
	DIFFTYPE dts{DTS_UNK};
	bool is_unk() { return (dts==DTS_UNK); } //e.g.: pipe, device, ..
	bool is_txt() { return (dts==DTS_TXT); }
	bool is_bin() { return (dts==DTS_BIN); }
	bool is_dir() { return (dts==DTS_DIR); }
	virtual ~DiffType()	{}
	virtual void set_difftype(DIFFTYPE dt)=0;
	virtual void clear()=0;
};

//===================================================================================================
struct FBlock
{
	FPos lpos;
	FPos rpos;
	FLine data;
	void clear()									{ lpos.clear(); rpos.clear(); data.clear(); }
	~FBlock()										{ clear(); }
	FBlock()										{ clear(); }
	FBlock(POSTYPE lp, POSTYPE rp, std::string d);
	FBlock(const FBlock &B)							{ *this=B; }
	FBlock& operator=(const FBlock &B);
	void add(POSTYPE lp, POSTYPE rp, std::string d);
	operator bool()									{ return !data.empty(); }
	size_t linecount()								{ return lpos.size(); }
};

//===================================================================================================
struct FDiff
{
	DIFFSTATE disp;
	FBlock block;
	void clear()									{ disp=DS_ERR; block.clear(); }
	~FDiff()										{ clear(); }
	FDiff()											{ clear(); }
	FDiff(DIFFSTATE dsp, const FBlock &B);
	FDiff(const FDiff &D)							{ *this=D; }
	FDiff& operator=(const FDiff &D)				{ disp=D.disp; block=D.block; return *this; }
	size_t lines()									{ return block.linecount(); }
};

using VFDiffs=std::vector<FDiff>;

//===================================================================================================
///enum DIFFTYPES { DTS_UNK=0, DTS_TXT, DTS_BIN, DTS_DIR, };
struct FDiffs : public DiffType
{
	VFDiffs vd;
	bool bLsorted{true};
	virtual void clear()					{ lpath.clear(); rpath.clear(); dts=DTS_UNK; vd.clear(); bLsorted=true; }
	virtual void set_difftype(DIFFTYPE dt)	{ dts=dt; }
	~FDiffs()								{ clear(); }
	FDiffs()								{ clear(); }
	void sort(bool bLeft=true)
	{
		if (!is_txt()||(bLeft==bLsorted)) return;
		bLsorted=bLeft;
		if (bLeft) std::sort(vd.begin(), vd.end(), [](const FDiff &L, const FDiff &R)->bool { return (L.block.lpos[0]<R.block.lpos[0]); });
		else std::sort(vd.begin(), vd.end(), [](const FDiff &L, const FDiff &R)->bool { return (L.block.rpos[0]<R.block.rpos[0]); });
	}
	size_t getlines() { size_t n=0; for (auto f:vd) n+=f.lines(); return n; }
};

//===================================================================================================
using NameDisp=std::map<std::string, DIFFSTATE>;
struct DDiff : public std::map<std::string, DDiff>
{
	DIFFSTATE disp;
	NameDisp content;
	virtual ~DDiff(){}
	void clear();
	DDiff() { clear(); }
	DDiff(const DDiff &D);
	DDiff& operator=(const DDiff &D);
	DDiff& set_sub(std::string name, DIFFSTATE disp);
	void set_content(std::string name, DIFFSTATE disp);
	void merge(const DirTree &dt, DIFFSTATE disp, PCBF pcb);

};

struct DDiffs : public DiffType
{
	DDiff dd{};
	virtual void clear()					{ lpath.clear(); rpath.clear(); dts=DTS_UNK; dd.clear(); }
	virtual void set_difftype(DIFFTYPE dt)	{ dts=dt; }
	
};

//===================================================================================================
bool DoDiff(std::string fl, std::string fr, DiffType **pDT, PCBF pcb=pcbdef); //returns true if no error



#endif
