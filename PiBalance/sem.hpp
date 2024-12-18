#ifndef SEM_HPP
#define SEM_HPP

#include <stdint.h>
#include <semaphore.h>

class TSemaphore
{
private:
	sem_t sem;
public:
	TSemaphore(uint32_t valeur = 1);
	~TSemaphore();

	void take(void);    //P
	void release(void); //V
};

#endif // SEM_HPP
