#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

#include <vector>

struct params
{
	int index;

	double cost;

	bool operator < (params p1)
	{
		if(cost < p1.cost)
			return true;

		else return false;
	}

	bool operator > (params p1)
	{
		if(cost > p1.cost)
			return true;

		else return false;
	}
};

template <class T>
class PriorityQueue
{
	std::vector<T> queue;

	void swap(T& p1, T& p2)
	{
		T temp;

		temp = p1; p1 = p2; p2 = temp;
	}

public:
	PriorityQueue() {}

	T* top()
	{
		if (!queue.empty())return &(queue.front()); 

		else return NULL;

	}

	void push(T pm)
	{
		queue.push_back(pm);

		reorderup();
	}

	void pop()
	{
		if(queue.empty()) return;

		queue.front() = queue.back();

		queue.pop_back();

		reorderdown();
	}

	bool empty() const {return queue.empty();}

	void reorderdown()
	{
		for(int j = 0, i = 0; i < (int)queue.size() / 2; i = j)
		{
			j = i * 2 + 1;

			if(( j + 1) < (int)queue.size()) if(queue[j] > queue[j+1]) j++;

			if(queue[i] > queue[j])
				swap(queue[i], queue[j]);
		}
	}

	void reorderup()
	{
		for(int i = 0, j = (int)queue.size() - 1; j > 0; j = i)
		{
			i = (j - 1) / 2;

			if(queue[j] < queue[i])
				swap(queue[i], queue[j]);	
		}
	}

	std::vector<T>* GetQueue () { return &queue; }

};


template <class T>
void ChangeCost(PriorityQueue<T> &pq,int index, double cost)
	{
		std::vector<T>::iterator iter;

		for(iter = pq.GetQueue()->begin(); iter != pq.GetQueue()->end(); ++iter)
		{
			if( (*iter).index == index )
			{
				(*iter).cost = cost;
				break;
			}
		}

		pq.reorderup();
		pq.reorderdown();
	}

#endif