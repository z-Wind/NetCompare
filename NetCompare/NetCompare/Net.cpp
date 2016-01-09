#include "StdAfx.h"
#include "Net.h"


CNet::CNet(void)
{
}


CNet::~CNet(void)
{
}

bool CNet::createPinNet(CString pinName,CString netName)
{
	for(unsigned int i=edgelist.size()-1; i>=0; i--)
	{
		if(edgelist[i].netName.Compare(netName) == 0)
		{
			edgelist[i].node.push_back(pinName);
			break;
		}
	}

	int	nTokenPos = 0;
	CString locaion;
	PINNET temp;

	temp.net = netName;
	locaion = pinName.Tokenize(".", nTokenPos);
	temp.pin = pinName.Tokenize(".", nTokenPos);
	
	for(unsigned int i=0; i<nodelist.size(); i++)
	{
		if(nodelist[i].location.Compare(locaion) == 0)
		{
			nodelist[i].pinNet.push_back(temp);
			return true;
		}
	}

	return false;
}

bool CNet::createMap(CString path)
{
	CStdioFile file;

	if(file.Open(path, CFile::modeRead) == FALSE)
	{
		return false;
	}

	CString valStr;
	CString strToken;
	int state = 0;
	int nTokenPos = 0;
	NODE node;
	EDGE edge;

	node.compared = false;
	edge.compared = false;

	nodelist.clear();
	edgelist.clear();

	while(file.ReadString(valStr))
	{
		if( valStr.Compare("*END*") == 0 )
		{
			break;
		}
		switch (state)
		{
			case 0://¤ÀÃþ
				if( valStr.Compare("*PART*") == 0 )
				{
					state = 1;
				}
				else if( valStr.Compare("*NET*") == 0 )
				{
					state = 2;
				}
				break;
			case 1://decal
				if(valStr.IsEmpty())
				{
					state = 0;
					break;
				}

				nTokenPos = 0;
				strToken = valStr.Tokenize(" ", nTokenPos);
				node.location = strToken;
				strToken = valStr.Tokenize(" ", nTokenPos);
				node.decal = strToken;

				nodelist.push_back(node);
				break;
			case 2://net
				if(valStr.IsEmpty())
				{
					state = 0;
					break;
				}

				if(valStr.Find("*SIGNAL*") != -1)
				{
					nTokenPos = 0;
					strToken = valStr.Tokenize(" ", nTokenPos);
					edge.netName = valStr.Tokenize(" ", nTokenPos);
					edgelist.push_back(edge);
				}
				else
				{
					nTokenPos = 0;
					strToken = valStr.Tokenize(" ", nTokenPos);
					while (!strToken.IsEmpty())
					{
						createPinNet(strToken,edge.netName);
						strToken = valStr.Tokenize(" ", nTokenPos);
					}
				}
				break;
			default:
				state = 0;
				break;
		}

	}

	file.Close();

	//check which decal has no net
	PINNET temp;
	temp.net = "¡iNONE¡j";
	temp.pin = "0";
	for(unsigned int i=0; i<nodelist.size(); i++)
	{
		if(nodelist[i].pinNet.size() == 0)
		{
			nodelist[i].pinNet.push_back(temp);
		}
	}

	return true;
}

bool CNet::compareDecal(NODE* a,NODE* b,Change& c)
{
	c.item = "";
	c.remove = "";
	c.add = "";
	c.extra[0] = "";
	c.extra[1] = "";

	if(b == NULL)
	{
		c.item.Format("%-6s", a->location);
		c.remove.Format("%-6s",a->decal);
		c.add.Format("%-6s","¡iNONE¡j");
		for(unsigned int i=0; i<a->pinNet.size() && i<2; i++)
		{
			c.extra[i].Format("%-15s", a->pinNet[i].net);
		}

		a->compared = true;
	}
	else if(a == NULL)
	{
		c.item.Format("%-6s", b->location);
		c.remove.Format("%-6s","¡iNONE¡j");
		c.add.Format("%-6s",b->decal);
		for(unsigned int i=0; i<b->pinNet.size() && i<2; i++)
		{
			c.extra[i].Format("%-15s", b->pinNet[i].net);
		}

		b->compared = true;
	}
	else if(a->location == b->location)
	{
		if(a->decal != b->decal)
		{
			c.item.Format("%-6s",a->location);
			c.remove.Format("%-6s",a->decal);
			c.add.Format("%-6s",b->decal);

			if( a->pinNet.size()>=2 && b->pinNet.size()>=2)
			{
				if(a->pinNet[0].net == b->pinNet[0].net && a->pinNet[1].net == b->pinNet[1].net)
				{
					c.extra[0].Format("%-15s", a->pinNet[0].net);
					c.extra[1].Format("%-15s", a->pinNet[1].net);
				}
				else
				{
					c.extra[0].Format("¡i%-15s -> %-15s¡j", a->pinNet[0].net, b->pinNet[0].net);
					c.extra[1].Format("¡i%-15s -> %-15s¡j", a->pinNet[1].net, b->pinNet[1].net);
				}
			}
			else
			{
				if(a->pinNet[0].net == b->pinNet[0].net)
				{
					c.extra[0].Format("%-15s", a->pinNet[0].net);
				}
				else
				{
					c.extra[0].Format("¡i%-15s -> %-15s¡j", a->pinNet[0].net, b->pinNet[0].net);
				}
			}
		}
		a->compared = true;
		b->compared = true;
	}
	
	return true;
}

CString CNet::ReportDecal(vector<NODE>& to)
{
	CString report = "";
	CString valStr;
	Change temp;
	vector<Change> change_record;
	change_record.clear();

	for(unsigned int i=0; i<nodelist.size(); i++)
	{
		if(nodelist[i].compared)
		{
			continue;
		}
		for(unsigned int j=0; j<to.size(); j++)
		{
			if(to[j].compared)
			{
				continue;
			}
			compareDecal(&nodelist[i],&to[j],temp);
			if(!temp.item.IsEmpty())
			{
				change_record.push_back(temp);
			}
		}
	}

	for(unsigned int i=0; i<nodelist.size(); i++)
	{
		if(!nodelist[i].compared)
		{
			compareDecal(&nodelist[i],NULL,temp);
			change_record.push_back(temp);
		}
	}

	for(unsigned int i=0; i<to.size(); i++)
	{
		if(!to[i].compared)
		{
			compareDecal(NULL,&to[i],temp);
			change_record.push_back(temp);
		}
	}

	//combine the same extra net
	for(unsigned int i=0; i<change_record.size(); i++)
	{
		int n = 1;
		CString net_temp;
		if(change_record[i].extra[1].IsEmpty() || change_record[i].extra[0].MakeUpper().Find("GND") ==-1)
		{	
			net_temp = change_record[i].extra[0];
		}
		else
		{
			net_temp = change_record[i].extra[1];
		}

		for(unsigned int j=i+1; j<change_record.size(); j++)
		{
			if(change_record[i].add == change_record[j].add
			&& change_record[i].remove == change_record[j].remove)
			{
				if(change_record[i].extra[1].IsEmpty() && change_record[j].extra[1].IsEmpty())
				{
					if(change_record[i].extra[0] == change_record[j].extra[0])
					{
						net_temp = change_record[i].extra[0];
					}
					else
					{
						continue;
					}
				}
				else if((change_record[i].extra[0] == change_record[j].extra[0] || change_record[i].extra[0] == change_record[j].extra[1])
				&& (change_record[i].extra[0].MakeUpper().Find("GND") == -1 || change_record[i].extra[0].Left(2) == "¡i") )
				{
					net_temp = change_record[i].extra[0];
				}
				else if((!change_record[i].extra[1].IsEmpty()) 
				&& (change_record[i].extra[1] == change_record[j].extra[1]  || change_record[i].extra[1] == change_record[j].extra[0])
				&& (change_record[i].extra[1].MakeUpper().Find("GND") == -1 || change_record[i].extra[1].Left(2) == "¡i") )
				{
					net_temp = change_record[i].extra[1];
				}				
				else if(change_record[i].extra[0].MakeUpper().Find("GND") != -1 
				&& change_record[i].extra[1].MakeUpper().Find("GND") != -1 
				&& change_record[j].extra[0].MakeUpper().Find("GND") != -1 
				&& change_record[j].extra[1].MakeUpper().Find("GND") != -1)
				{
					net_temp = change_record[i].extra[0];
				}
				else
				{
					continue;
				}

				/*if((change_record[i].extra[0] == change_record[j].extra[0] && change_record[i].extra[1] == change_record[j].extra[1])
				|| (change_record[i].extra[0] == change_record[j].extra[1] && change_record[i].extra[1] == change_record[j].extra[0]))
				{*/
					n++;
					if(n > 15)
					{
						valStr.Format(",\r\n%-18s%s","",change_record[j].item);
						n = 1;
					}
					else
					{
						valStr.Format(",%s",change_record[j].item);
					}
					change_record[i].item += valStr;

					change_record.erase(change_record.begin()+j);
					j--;
				//}
			}
		}
		change_record[i].item = net_temp + " ~ " + change_record[i].item;
	}

	//combine the same add & remove
	for(unsigned int i=0; i<change_record.size(); i++)
	{
		bool last = false;
		for(unsigned int j=i+1; j<change_record.size(); j++)
		{
			if(change_record[i].add == change_record[j].add
			&& change_record[i].remove == change_record[j].remove)
			{
				if ((change_record[i].item.Left(2) == "¡i" && change_record[j].item.Left(2) == "¡i")
				||  (change_record[j].item.Left(2) != "¡i"))
				{
					change_record[i].item = change_record[i].item + "\r\n" + change_record[j].item;	
					change_record.erase(change_record.begin()+j);
					j--;	
				}
				else
				{
					last = true;
				}
			}
		}
		if(!last)
		{
			continue;
		}
		for(unsigned int j=i+1; j<change_record.size(); j++)
		{
			if(change_record[i].add == change_record[j].add
			&& change_record[i].remove == change_record[j].remove)
			{
				change_record[i].item = change_record[i].item + "\r\n" + change_record[j].item;	
				change_record.erase(change_record.begin()+j);
				j--;	
			}
		}
	}

	for(unsigned int i=0; i<change_record.size(); i++)
	{
		report += "¡» " + change_record[i].remove + " => "  + change_record[i].add + "\r\n";
		report += "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n";
		report += change_record[i].item;
		report += "\r\n\r\n";
	}

	return report;
}

bool CNet::compareNet(EDGE* a,EDGE* b,Change& c)
{
	CString valStr,temp;
	int n;
	const int N = 15;

	c.item = "";
	c.remove = "";
	c.add = "";
	c.extra[0] = "";
	c.extra[1] = "";

	if(b == NULL)
	{
		c.item = a->netName + " => ¡¹Removed";

		valStr = "";
		n = 1;
		for(unsigned int i=0; i<a->node.size(); i++)
		{
			n++;
			if(n > N)
			{
				temp.Format("%s,\r\n%6s%-8s",valStr,"", a->node[i]);
				n = 1;
			}
			else
			{
				temp.Format("%s,%-8s",valStr, a->node[i]);
			}
			valStr = temp;
		}
		valStr.Delete(0,1);
		c.remove.Format("%6s%s","R <-- ",valStr);

		c.add = "A --> ¡iNONE¡j";
		
		a->compared = true;
	}
	else if(a == NULL)
	{
		c.item = b->netName + " => ¡¹Added";

		c.remove = "R <-- ¡iNONE¡j";

		valStr = "";
		n = 1;
		for(unsigned int i=0; i<b->node.size(); i++)
		{
			n++;
			if(n > N)
			{
				temp.Format("%s,\r\n%6s%-8s",valStr,"", b->node[i]);
				n = 1;
			}
			else
			{
				temp.Format("%s,%-8s",valStr, b->node[i]);
			}
			valStr = temp;
		}
		valStr.Delete(0,1);
		c.add.Format("%6s%s","A --> ",valStr);
		
		b->compared = true;
	}
	else if(a->netName == b->netName)
	{		
		for(unsigned int i=0; i<a->node.size(); i++)
		{
			for(unsigned int j=0; j<b->node.size(); j++)
			{
				if(a->node[i] == b->node[j])
				{
					a->node.erase(a->node.begin()+i);
					b->node.erase(b->node.begin()+j);
					i--;
					j--;
					break;
				}
			}
		}

		a->compared = true;
		b->compared = true;

		if( a->node.size() == 0 && b->node.size() == 0)
		{
			return true;
		}

		c.item = a->netName;

		valStr = "";
		n = 1;
		for(unsigned int i=0; i<a->node.size(); i++)
		{
			n++;
			if(n > N)
			{
				temp.Format("%s,\r\n%6s%-8s",valStr,"", a->node[i]);
				n = 1;
			}
			else
			{
				temp.Format("%s,%-8s",valStr, a->node[i]);
			}
			valStr = temp;
		}
		valStr.Delete(0,1);
		if(valStr.IsEmpty())
		{
			c.remove = "R <-- ¡iNONE¡j";
		}
		else
		{
			c.remove.Format("%6s%s","R <-- ",valStr);
		}

		valStr = "";
		n = 1;
		for(unsigned int i=0; i<b->node.size(); i++)
		{
			n++;
			if(n > N)
			{
				temp.Format("%s,\r\n%6s%-8s",valStr,"", b->node[i]);
				n = 1;
			}
			else
			{
				temp.Format("%s,%-8s",valStr, b->node[i]);
			}
			valStr = temp;
		}
		valStr.Delete(0,1);
		if(valStr.IsEmpty())
		{
			c.add = "A --> ¡iNONE¡j";
		}
		else
		{
			c.add.Format("%6s%s","A --> ",valStr);
		}
	}
	return true;
}

CString CNet::ReportNet(vector<EDGE>& to)
{
	CString report = "";
	Change temp;
	vector<Change> change_record;
	change_record.clear();

	for(unsigned int i=0; i<edgelist.size(); i++)
	{
		if(edgelist[i].compared)
		{
			continue;
		}
		for(unsigned int j=0; j<to.size(); j++)
		{
			if(to[j].compared)
			{
				continue;
			}
			compareNet(&edgelist[i],&to[j],temp);
			if(!temp.item.IsEmpty())
			{
				change_record.push_back(temp);
			}
		}
	}

	for(unsigned int i=0; i<edgelist.size(); i++)
	{
		if(!edgelist[i].compared)
		{
			compareNet(&edgelist[i],NULL,temp);
			change_record.push_back(temp);
		}
	}

	for(unsigned int i=0; i<to.size(); i++)
	{
		if(!to[i].compared)
		{
			compareNet(NULL,&to[i],temp);
			change_record.push_back(temp);
		}
	}

	//combine
	int n = 0;
	for(unsigned int i=0; i<change_record.size(); i++)
	{
		for(unsigned int j=i+1; j<change_record.size(); j++)
		{
			if(change_record[i].add == change_record[j].add
			&& change_record[i].remove == change_record[j].remove)
			{
				n++;
				if(n > 20)
				{
					change_record[i].item = change_record[i].item + ",\r\n" + change_record[j].item;	
					n = 1;
				}
				else
				{
					change_record[i].item = change_record[i].item + ", " + change_record[j].item;
				}
				change_record.erase(change_record.begin()+j);
				j--;	
			}
		}
	}

	for(unsigned int i=0; i<change_record.size(); i++)
	{
		report += "¡» " + change_record[i].item + "\r\n";
		report += "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n";
		report += change_record[i].remove;
		report += "\r\n=================\r\n";
		report += change_record[i].add;
		report += "\r\n\r\n";
	}

	return report;
}