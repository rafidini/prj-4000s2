/*
  Who wrote this ??

  Question by Hugues Talbot	18 Aug 2010
 */

#ifndef _FIFO_HPP_
#define _FIFO_HPP_
template <typename T>
class FIFO{
	private:
		T* buffer;
		unsigned int maxSize;
		unsigned int front;
		unsigned int back;



	public:
			//Constructor
			inline FIFO(unsigned int maxSize){
				buffer = new T[maxSize];
				this->maxSize = maxSize;
				front = 0;
				back = 0;
			}
			//Destructor
			inline ~FIFO(){
				delete[] buffer;
			}
			
			inline bool empty(){
				return front == back;
			}

			inline void push(T val){
				buffer[back++] = val;
				if(back>=maxSize)back = 0;
			}

			inline T pop(){
				T val = buffer[front++];
				if(front>=maxSize)front = 0;
				return val;
			}

			inline unsigned int size(){
				if(front <= back)
					return back-front;
				return back+maxSize-front;
			}

			inline void clear(){
				front = 0;
				back = 0;
			}
};

#endif
