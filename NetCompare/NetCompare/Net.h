#pragma once

#include <vector>
using std::vector; 

typedef struct  _PINNET
{
	CString pin;
	CString net;
} PINNET;

typedef struct _NODE
{
	CString location;
	CString decal;
	vector<PINNET> pinNet;
	bool compared;

} NODE;

typedef struct _EDGE
{
	CString netName;
	vector<CString> node;
	bool compared;

} EDGE;

typedef struct _Change
{
	CString item;

	CString remove;
	CString add;

	CString extra[2];
}Change;

class CNet
{	
private:
	bool compareDecal(NODE* a,NODE* b,Change& c);
	bool compareNet(EDGE* a,EDGE* b,Change& c);
public:
	vector<NODE> nodelist;
	vector<EDGE> edgelist;

	CNet(void);
	~CNet(void);
	bool createMap(CString path);
	bool createPinNet(CString pinName,CString netName);
	CString ReportDecal(vector<NODE>& to);
	CString ReportNet(vector<EDGE>& to);
};

