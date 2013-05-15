#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include "Vector3D.h"
#include <queue>
#include <list>
#include <cmath>
#include "PriorityQueue.h"

enum node_type{invalid_node = -1};

class GraphNode
{
private:
	int m_iIndex;

	Vector3D m_pPosition;

public:
	GraphNode()
	{
		m_iIndex = invalid_node;
		m_pPosition = Vector3D();
	}

	GraphNode(int ind) 
	{
		m_iIndex = ind;
		m_pPosition = Vector3D();
	}

	void SetIndex(int ind)
	{
		m_iIndex = ind;
	}

	int Index()
	{
		return m_iIndex;
	}

	void SetPosition(Vector3D pos)
	{
		m_pPosition = pos;
	}

	Vector3D Position()
	{
		return m_pPosition;
	}
};

class GraphEdge
{
private:
	int m_iFrom;
	int m_iTo;

	double m_dCost;

public:
	GraphEdge(int from, int to, double cost) : m_iFrom(from),
											m_iTo(to),
											m_dCost(cost)
	{}

	GraphEdge(int from, int to) : m_iFrom(from),
								  m_iTo(to),
								  m_dCost(1.0)
	{}

	GraphEdge() : m_iFrom(invalid_node),
				  m_iTo(invalid_node),
				  m_dCost(1.0)
	{}

	void SetFrom(int from) {m_iFrom = from;}
	int From() {return m_iFrom;}

	void SetTo(int to) {m_iTo = to;}
	int To() {return m_iTo;}

	void SetCost(double cost) {m_dCost = cost;}
	double Cost() {return m_dCost;}
};

//class Dijkstra;
class WorldGraph
{
private:

	std::vector<GraphNode> Nodes;

	std::vector<std::list<GraphEdge>> EdgeList;

	//Dijkstra Dj;

public:
	WorldGraph() {}
	WorldGraph(int sizex, int sizez)
	{
		GraphNode node;

		std::list<GraphEdge> list;

		for(int i = 0; i < sizex; i++)
		{
			for(int j = 0; j < sizez; j++)
			{
				
				Nodes.push_back(node);

				EdgeList.push_back(list);
			}
		}
	}
	~WorldGraph() {}

	GraphNode GetNode(int index) {return Nodes[index];}

	GraphEdge GetEdge(int from, int to)
	{
		std::list<GraphEdge>::iterator iter;

		for (iter = EdgeList[from].begin(); iter != EdgeList[from].end(); ++iter)
		{
			if ((*iter).To() == to)
				return (*iter);
		}

		return GraphEdge();
	}

	bool AddNode(GraphNode gn)
	{
		if((unsigned)gn.Index() < Nodes.size())
		{
			Nodes[gn.Index()] = gn;

			return true;
		}

		else return false;
	}

	bool AddEdge(GraphEdge edge)
	{
		if((unsigned)edge.From() < EdgeList.size())
		{
			EdgeList[edge.From()].push_back(edge);

			return true;
		}

		return false;
	}

	void ClearGraph()
	{
		while(!Nodes.empty())
		{
			Nodes.pop_back();
		}

		while(!EdgeList.empty())
		{
			EdgeList.pop_back();
		}
	}

	void ViewEdge(int index)
	{
		ofstream outfile;
		outfile.open("hamro patru daju bhai.txt", ios::app);
		outfile << index << endl;

		std::list<GraphEdge>::iterator iter;

		for(iter = EdgeList[index].begin(); iter != EdgeList[index].end(); ++iter)
		{
			outfile << (*iter).To() << "   ";
		}

		outfile << endl << endl << endl;

		outfile.close();
	}

	std::list<GraphEdge>* GetEdgeList(int index)
	{
		return &EdgeList[index];
	}

	int NumNodes()
	{
		return Nodes.size();
	}

};

class DistBetweenNodes
{
public:
	double DistNodes(int n1, int n2)
	{
		int x1 = n1 % 100;
		int y1 = n1 / 100;

		int x2 = n2 % 100;
		int y2 = n2 / 100;

		return  ( (double) (sqrtf ( (float) (x2 - x1) * (x2 - x1) + (float) (y2 - y1) * (y2 - y1) ) ));
	}
};

class Dijkstra
{
	PriorityQueue<params> pq;

	WorldGraph *Graph;

	std::vector<double> m_cGCost;

	std::vector<double> m_cFCost;

	std::vector<GraphEdge*> m_cShortestPathTree;

	std::vector<GraphEdge*> m_cSearchFrontier;

	int m_iSource;
	int m_iTarget;

	void Search()
	{

		DistBetweenNodes Dist;
		params pm;

		pm.index = m_iSource;
		pm.cost = 0.0 + Dist.DistNodes(m_iSource, m_iTarget);

		GraphEdge edge(m_iSource, m_iSource, 0.0);
		m_cSearchFrontier[m_iSource] = &edge;
		m_cGCost[m_iSource] = 0.0;
		m_cFCost[m_iSource] = pm.cost;

		pq.push(pm);

		while(!pq.empty())
		{
			params next = *(pq.top());

			pq.pop();

			int next_node = next.index;

			m_cShortestPathTree[next_node] = m_cSearchFrontier[next_node];

			if(next_node == m_iTarget) return;


			std::list<GraphEdge>::iterator iter;

			iter = Graph->GetEdgeList(next_node)->begin();

			while(iter != Graph->GetEdgeList(next_node)->end())
			{
				double GCost = m_cGCost[next_node] + iter->Cost();

				double FCost = Dist.DistNodes(next_node, m_iTarget);

				if(m_cSearchFrontier[iter->To()] == 0)
				{
					m_cFCost[iter->To()] = GCost + FCost;

					m_cGCost[iter->To()] = GCost;

					next.index = iter->To();

					next.cost = FCost + GCost;

					pq.push(next);
			
					m_cSearchFrontier[iter->To()] = &(*iter);
				}

				else if(GCost < m_cGCost[iter->To()] && m_cShortestPathTree[iter->To()] == 0)
				{

					m_cFCost[iter->To()] = GCost + FCost;

					m_cGCost[iter->To()] = GCost;

					ChangeCost(pq, iter->To(), FCost + GCost);

					m_cSearchFrontier[iter->To()] = &(*iter);
				}

				++iter;
			}

		}
	}

public:
	Dijkstra() {}

	Dijkstra(WorldGraph *WG, int src, int tar) : Graph(WG),
												 m_cGCost(WG->NumNodes()),
												  m_cFCost(WG->NumNodes()),
												 m_cShortestPathTree(WG->NumNodes()),
												 m_cSearchFrontier(WG->NumNodes()),
												  m_iSource(src),
												 m_iTarget(tar)

	{
		Search();
	}

std::list<int> GetPathToTarget()
{
	std::list<int> path;

	if(m_iTarget < 0) return path;

	int tar = m_iTarget;

	path.push_back(tar);

	while(tar != m_iSource)
	{
		tar = m_cShortestPathTree[tar]->From();

		path.push_back(tar);	
	}

	return path;
}
	

};
#endif