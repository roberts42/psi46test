// connect.h

#ifndef CONNECT_H
#define CONNECT_H

#include <exception>


// === data pipe error ======================================================

class DataPipeException : public std::exception
{
public:
	DataPipeException(const char *message) : std::exception() {};
	//DataPipeException(const char *message) : std::exception(message) {};

};

class DP_not_connected : public DataPipeException
{
public:
	DP_not_connected() : DataPipeException("Not connected") {};
};



// === data source ==========================================================

// The inheritor must define ReadLast and Read
template <class T>
class CSource
{
	virtual T ReadLast() = 0;
	virtual T Read() = 0;
public:
	virtual ~CSource() {}

	template <class S> friend class CSink;
};

// Null source for not connected sinks
template <class T>
class CNullSource : public CSource<T>
{
protected:
	CNullSource() {}
	CNullSource(const CNullSource&) {}
	~CNullSource() {}
    T ReadLast() { throw DP_not_connected(); }
	T Read()     { return ReadLast();     }
	template <class TO> friend class CSink;
};


// === data sink ============================================================

template <class TI, class TO> class CDataPipe;

template <class T>
class CSink
{
protected:
	CSource<T> *src;
	static CNullSource<T> null;
public:
	CSink() : src(&null) {}

	T GetLast() { return src->ReadLast(); }
	T Get()     { return src->Read();     }
	void GetAll() { while (true) Get(); }
	template <class TI, class TO> friend void operator >> (CSource<TI> &, CSink<TO> &);
	template <class TI, class TO> friend CSource<TO>& operator >> (CSource<TI> &in, CDataPipe<TI,TO> &out);
};

template <class T> CNullSource<T> CSink<T>::null;


// === data in -> out (pipe) ================================================

template <class TI, class TO=TI>
class CDataPipe : public CSink<TI>, public CSource<TO> {};


// === operators to connect pipes ===========================================

// source -> sink; source -> datapipe
template <class TI, class TO>
void operator >> (CSource<TI> &in, CSink<TO> &out)
{
	out.src = &in;
}

// source -> datapipe -> datapipe -> sink
template <class TI, class TO>
CSource<TO>& operator >> (CSource<TI> &in, CDataPipe<TI,TO> &out)
{
	out.src = &in;
	return out;
}


#endif
