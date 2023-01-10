#pragma once
#include <iostream>
#include <fstream>
using namespace std;
namespace Voting {
	template<class T>
	class List
	{
		class Node
		{
		public:
			T data;
			Node* next;
			Node* prev;
			Node(const T& data, Node* next, Node* prev) :data(data), next(next), prev(prev) {}
		};
	private:
		Node* head;
		Node* tail;
		int length;
		Node* createNewNode(const T& data, Node* next, Node* prev)
		{
			try 
			{ 
				Node* newNode = new Node{ data, next, prev };
				return newNode;
			}
			catch (bad_alloc& ex)
			{ 
				throw ex; 
			}
			
		}
	public:
		List() :head(nullptr), tail(nullptr), length(0) {}
		~List()
		{
			this->clear();
		}
	public:
		class iterator
		{
		private:
			Node* node;
		public:
			friend class const_iterator;
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			iterator(Node* node) : node(node) {}
			iterator(const iterator& other) : node(other.node) {}			

			const iterator& operator=(const iterator& other)
			{
				node = other.node;
				return *this;
			}
			bool operator==(const iterator& other) const
			{
				return (node == other.node);
			}
			bool operator!=(const iterator& other) const
			{
				return !(*this == other.node);
			}
			T operator*()
			{
				return node->data;
			}
			T* operator->()
			{
				return &(node->data);
			}
			iterator& operator++() {
				this->node = this->node->next;
				return *this;
			}
			iterator operator++(int)
			{
				iterator temp(*this);
				this->node = this->node->next;
				return temp;
			}
			iterator& operator--()
			{
				this->node = this->node->prev;
				return *this;
			}
			iterator operator--(int)
			{
				iterator temp(*this);
				this->node = this->node->prev;
				return temp;
			}
		};
		class const_iterator
		{
		private:
			const Node* node;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = Node;
			using pointer = Node*;
			using reference = Node&;

			const_iterator(const Node* node) :node(node) {}
			const_iterator(const iterator& other) :node(other.node) {}
			const_iterator(const_iterator& other) :node(other.node) {}
			const_iterator& operator=(const iterator& other)
			{
				this->node = other.node;
				return *this;
			}
			const_iterator& operator=(const_iterator& other)
			{
				this->node = other.node;
				return *this;
			}
			bool operator==(const_iterator& other) const
			{
				return (node == other.node);
			}
			bool operator!=(const_iterator& other) const
			{
				return !(*this == other);
			}
			const T& operator*()
			{
				return *node;
			}
			const T* operator->()
			{
				return node;
			}
			const_iterator& operator++() {
				this->node = this->node->next;
				return *this;
			}
			const_iterator operator++(int)
			{
				iterator temp(*this);
				this->node = this->node->next;
				return temp;
			}
			const_iterator& operator--()
			{
				this->node = this->node->prev;
				return *this;
			}
			const_iterator operator--(int)
			{
				iterator temp(*this);
				this->node = this->node->prev;
				return temp;
			}
		};
		const List& operator=(const List& list)
		{
			Node* curr = list.head;
			while (curr != nullptr)
			{
				push_back(curr->data);
				curr = curr->next;
			}
			this->length = list.length;
			return *this;
		}
		bool operator==(const List& list)
		{
			Node* curr = this->head;
			Node* currList = list.head;

			bool flag = true;
			if (this->length != list.length)
				flag = false;
			while (curr != nullptr && flag)
			{
				if (curr->data != currList->data)
					flag = false;
				curr = curr->next;
				currList = currList->next;
			}
			return flag;
		}
		iterator begin()const
		{
			return iterator(this->head);
		}
		iterator end()const
		{
			return iterator(nullptr);
		}
		iterator findByIndex(int index) const
		{
			iterator iter(this->head);
			for (int i = 1; i < index; i++)
			{
				iter++;
			}
			return iter;
		}
		const iterator cbegin()const
		{
			return this->begin();
		}
		const iterator cend()const
		{
			return this->end();
		}
		List(const List& list)
		{
			*this = list;
		}
		Node* getHead() const
		{
			return this->head;
		}
		Node* getTail() const
		{
			return this->tail;
		}
		void push_back(const T& data)
		{
			Node* newNode = createNewNode(data, nullptr, this->tail);
			if (empty())
			{
				this->tail = newNode;
				this->head = newNode;
			}
			else
			{
				this->tail->next = newNode;
				this->tail = newNode;
			}
			(this->length)++;
		}
		void push_front(const T& data)
		{
			Node* newNode = createNewNode(data, this->head, nullptr);
			if (empty())
			{
				this->tail = newNode;
				this->head = newNode;
			}
			else
			{
				this->head->prev = newNode;
				this->head = newNode;
			}
			(this->length)++;
		}
		void pop_back()
		{
			Node* curr = this->tail;
			if (!empty())
			{
				if (this->length == 1)
				{
					delete curr;
					this->length = 0;
					this->head = nullptr;
					this->tail = nullptr;
				}
				else
				{
					this->tail = curr->prev;
					delete curr;
					this->tail->next = nullptr;
					(this->length)--;
				}
			}
		}
		void pop_front()
		{
			Node* curr = this->head;
			if (!empty())
			{
				if (this->length == 1)
				{
					delete curr;
					this->length = 0;
					this->head = nullptr;
					this->tail = nullptr;
				}
				else
				{
					this->head = curr->next;
					delete curr;
					this->head->prev = nullptr;
					(this->length)--;
				}
			}
		}
		iterator insert(const iterator& pos, const T& data)
		{
			Node* newNode = createNewNode(data, pos.node->next, pos.node);

			if (pos.node->next == nullptr)
			{
				push_back(data);
			}
			else
			{
				push_front(data);
			}
			return iterator(newNode);
		}
		const iterator& erase(const iterator& iter)
		{
			if (iter.node == this->head)
			{
				pop_front();
				return iterator(this->head);
			}
			else if (iter.node == this->tail)
			{
				pop_back();
				return iterator(this->tail);
			}
			else
			{
				Node* curr = iter.node;
				Node* temp = curr.next;
				curr->prev->next = temp;
				curr->next->prev = curr->prev;
				delete curr;
				return iterator(temp);
			}
		}
		const iterator& erase(const iterator& first, const iterator& last)
		{
			iterator& temp = first;
			for (auto curr = first; curr != last; ++curr)
			{
				temp = erase(curr);
			}
			return temp;
		}
		bool empty() const
		{
			return this->length == 0;
		}
		T front() const
		{
			return this->head->data;
		}
		T back()
		{
			return this->tail->data;
		}
		int size() const
		{
			return this->length;
		}
		void clear()
		{
			Node* curr = head;
			while (curr != nullptr)
			{
				curr = curr->next;
				delete head;
				head = curr;
			}
			this->head = nullptr;
			this->tail = nullptr;
			this->length = 0;
		}		
		
	};
}
