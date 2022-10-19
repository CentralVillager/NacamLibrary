#pragma once

template <typename T>

class Singleton
{
protected:

	Singleton() {};
	virtual ~Singleton() {};
	Singleton(const Singleton &obj) = delete;
	Singleton &operator=(const Singleton &obj) = delete;

public:

	static T *GetInstance()
	{
		static T instance;
		return &instance;
	};
};
