#include "fdd.h"
#include <fstream>
#include <fstab.h>
//#include <utilfuncs/utilfuncs.h>
#include <map>

//===================================================================================================
using VS=std::vector<std::string>;

//===================================================================================================
FBlock::FBlock(POSTYPE lp, POSTYPE rp, std::string d) { clear(); add(lp, rp, d); }

FBlock& FBlock::operator=(const FBlock &B)
{
	clear();
	if ((B.lpos.size()>0)&&(B.lpos.size()==B.rpos.size())&&(B.lpos.size()==B.data.size()))
	{
		for (size_t i=0; i<B.lpos.size(); i++)
		{
			lpos.push_back(B.lpos[i]);
			rpos.push_back(B.rpos[i]);
			data.push_back(B.data[i]);
		}
	}
	return *this;
}

void FBlock::add(POSTYPE lp, POSTYPE rp, std::string d)
{
	lpos.push_back(lp);
	rpos.push_back(rp);
	data.push_back(d);
}

//===================================================================================================
FDiff::FDiff(DIFFSTATE dsp, const FBlock &B) { disp=dsp; block=B; }

void match_text_data(VFDiffs &vd, const VS &vl, const VS &vr, PCBF pcb)
{
	FBlock B{};
	std::map<POSTYPE, bool> mskip{}; //list of right-lines that are matched
	POSTYPE pl=0, pr=0, lf{INVALID_POS};
	auto skip=[&mskip](POSTYPE n)->bool{ return (mskip.find(n)!=mskip.end()); };
	auto cpr=[&skip, &lf, &vr, &pr](){ if (skip(pr)||(pr==lf)) do pr++; while ((pr<(POSTYPE)vr.size())&&(skip(pr)||(pr==lf))); };
	while (pl<(POSTYPE)vl.size()) //match all of left to right..
	{
		//pcb(pl, vl.size(), "Comparing...");
		if (seqs(vl[pl], vr[pr]))
		{
			B.clear();
			while ((pl<(POSTYPE)vl.size())&&(pr<(POSTYPE)vr.size())&&!skip(pr)&&seqs(vl[pl], vr[pr]))
			{
				pcb(pl, vl.size(), "Comparing...");
				B.add(pl, pr, vl[pl]); mskip[pr]=true;
				pl++; pr++; //must match contiguously to be a block
			}
			if (B) { vd.push_back(FDiff((pl==pr)?DS_EQU:DS_MOV, B)); }
			lf=pr; pr=0; cpr();
		}
		else
		{
			pcb(pl, vl.size(), "Comparing...");
			lf=pr; cpr();
			if (pr>=(POSTYPE)vr.size()) { vd.push_back(FDiff(DS_DEL, FBlock(pl, INVALID_POS, vl[pl]))); pl++; pr=0; cpr(); }
		}
	}
	pr=0; //get remainder of right..
	while (pr<(POSTYPE)vr.size()) { if (!skip(pr)) { vd.push_back(FDiff(DS_ADD, FBlock(INVALID_POS, pr, vr[pr]))); } pr++; }
}

//===================================================================================================
void match_bin_data(VFDiffs &vd, const VS &vl, const VS &vr, PCBF pcb)
{
	POSTYPE p=0, m;
	m=(POSTYPE)std::max(vl.size(), vr.size());
	while (p<m)
	{
		if (p>=vl.size()) { while (p<vr.size()) { pcb(p, m, "Comparing..."); vd.push_back(FDiff(DS_NEW, FBlock(p, p, vr[p]))); p++; }}
		else if (p>=vr.size()) { while (p<vl.size()) { pcb(p, m, "Comparing..."); vd.push_back(FDiff(DS_OLD, FBlock(p, p, vl[p]))); p++; }}
		else //p<both
		{
			pcb(p, m, "Comparing...");
			if (seqs(vl[p], vr[p])) { vd.push_back(FDiff(DS_EQU, FBlock(p, p, vl[p]))); }
			else { std::string s=spf(vl[p].size(), ':', vl[p], vr[p]); vd.push_back(FDiff(DS_NEQ, FBlock(p, p, s))); }
			p++;
		}
	}
}

//===================================================================================================
void DDiff::clear() { std::map<std::string, DDiff>::clear(); content.clear(); disp=DS_ERR; }
	
DDiff::DDiff(const DDiff &D) { *this=D; }

DDiff& DDiff::operator=(const DDiff &D)
{
	clear();
	for (auto d:D) (*this)[d.first]=d.second;
	disp=D.disp;
	for (auto c:D.content) content[c.first]=c.second;
	return *this;
}

DDiff& DDiff::set_sub(std::string name, DIFFSTATE disp)
{
	if (find(name)!=end()) { (*this)[name].disp=DS_EQU; }
	else (*this)[name].disp=disp;
	return (*this)[name];
}

void DDiff::set_content(std::string name, DIFFSTATE disp)
{
	if (content.find(name)!=content.end()) { content[name]=DS_EQU; }
	else content[name]=disp;
}

void DDiff::merge(const DirTree &dt, DIFFSTATE disp, PCBF pcb)
{
	for (auto pp:dt.content) { pcb(0,0,pp.first); set_content(pp.first, disp); }
	for (auto p:dt)
	{
		pcb(0,0,p.first);
		DDiff &dd=set_sub(p.first, disp);
		dd.merge(p.second, disp, pcb);
	}
}

void match_dir_data(DDiff &DD, const DirTree &dl, const DirTree &dr, PCBF pcb)
{
	DD.clear();
	DD.disp=DS_EQU;
	DD.merge(dl, DS_LEFT, pcb);
	DD.merge(dr, DS_RIGHT, pcb);
}


//===================================================================================================
void diff_text(DiffType *pDT, PCBF pcb) //low-fidelity, lines-only matching..
{
	std::string s;
	VS vl{}; file_read(pDT->lpath, s); splitslist(s, '\n', vl); s.clear(); ///todo:...make file_read_lines(f, v); in utilities...
	VS vr{}; file_read(pDT->rpath, s); splitslist(s, '\n', vr); s.clear();
	match_text_data(((FDiffs*)pDT)->vd, vl, vr, pcb);
}

void diff_binary(DiffType *pDT, PCBF pcb) //low fidelity, match only if offset&lines ==, else del/insert..
{
	std::string s;
	VS vl{}; file_read(pDT->lpath, s); splitslen(s, 16, vl); s.clear();
	VS vr{}; file_read(pDT->rpath, s); splitslen(s, 16, vr); s.clear();
	match_bin_data(((FDiffs*)pDT)->vd, vl, vr, pcb);
}

void diff_directory(DiffType *pDT, PCBF pcb)  //low fidelity, match only names ==, else del/insert or sub-dir chg if any content-name diffs
{
	DirTree dl; dir_read_deep(pDT->lpath, dl);
	DirTree dr; dir_read_deep(pDT->rpath, dr);
	match_dir_data(((DDiffs*)pDT)->dd, dl, dr, pcb);
}

//===================================================================================================
bool DoDiff(std::string f1, std::string f2, DiffType **pDT, PCBF pcb)
{
	if (isfile(f1)&&isfile(f2))
	{
		(*pDT)=new FDiffs; (*pDT)->clear();
		(*pDT)->lpath=f1; (*pDT)->rpath=f2;
		if (istextfile(f1)&&istextfile(f2)) { (*pDT)->set_difftype(DTS_TXT); diff_text(*pDT, pcb); }
		else { (*pDT)->set_difftype(DTS_BIN); diff_binary(*pDT, pcb); }
	}
	if (isdir(f1)&&isdir(f2))
	{
		(*pDT)=new DDiffs; (*pDT)->clear();
		(*pDT)->lpath=f1; (*pDT)->rpath=f2;
		(*pDT)->set_difftype(DTS_DIR);
		diff_directory(*pDT, pcb);
	}
	return ((*pDT)!=nullptr);
}

