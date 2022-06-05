#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <irrlicht.h>

class IPointer
{
public:
    IPointer(void)
    {
        ref = 1;
    }
    virtual ~IPointer(void)
    {

    }
    void grab(void)
    {
        ref++;
    }
    void drop(void)
    {
        ref--;
        if (ref == 0)
        {
            delete this;
        }
        else if (ref < 0)
        {
            exit(1);
        }
    }
protected:
    int ref;
};
class SFunctor : public IPointer
{
public:
    /** Default constructor */
    SFunctor() {}
    /** Default destructor */
    virtual ~SFunctor() {}

    virtual void operator ()()=0;

    virtual void call()=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};


template<typename Return>
class SFunctorR : public IPointer
{
public:
    /** Default constructor */
    SFunctorR() {}
    /** Default destructor */
    virtual ~SFunctorR() {}

    virtual Return operator ()()=0;

    virtual Return call()=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename A>
class SFunctor1 : public IPointer
{
public:
    /** Default constructor */
    SFunctor1() {}
    /** Default destructor */
    virtual ~SFunctor1() {}

    virtual void operator ()(A var)=0;

    virtual void call(A var)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename Return, typename A>
class SFunctorR1 : public IPointer
{
public:
    /** Default constructor */
    SFunctorR1() {}
    /** Default destructor */
    virtual ~SFunctorR1() {}

    virtual Return operator ()(A var)=0;

    virtual Return call(A var)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename A, typename B>
class SFunctor2 : public IPointer
{
public:
    /** Default constructor */
    SFunctor2() {}
    /** Default destructor */
    virtual ~SFunctor2() {}

    virtual void operator ()(A var, B var2)=0;

    virtual void call(A var, B var2)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename Return, typename A, typename B>
class SFunctorR2 : public IPointer
{
public:
    /** Default constructor */
    SFunctorR2() {}
    /** Default destructor */
    virtual ~SFunctorR2() {}

    virtual Return operator ()(A var, B var2)=0;

    virtual Return call(A var, B var2)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename A, typename B, typename C>
class SFunctor3 : public IPointer
{
public:
    /** Default constructor */
    SFunctor3() {}
    /** Default destructor */
    virtual ~SFunctor3() {}

    virtual void operator ()(A var, B var2, C var3)=0;

    virtual void call(A var, B var2, C var3)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

template<typename Return, typename A, typename B, typename C>
class SFunctorR3 : public IPointer
{
public:
    /** Default constructor */
    SFunctorR3() {}
    /** Default destructor */
    virtual ~SFunctorR3() {}

    virtual Return operator ()(A var, B var2, C var3)=0;

    virtual Return call(A var, B var2, C var3)=0;

    virtual void* getClassPointer(void) = 0;
protected:
private:
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class SFunctorReciever : public SFunctor
{
protected:
    typedef void (T::*funcType)();
    funcType Callback;
    T* CallbackClass;
public:

    SFunctorReciever(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorReciever(const SFunctorReciever& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    void operator ()(void)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)();
    }

    void call(void)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)();
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename Return, typename T>
class SFunctorRecieverR : public SFunctorR<Return>
{
protected:
    typedef Return (T::*funcType)();
    funcType Callback;
    T* CallbackClass;
public:

    SFunctorRecieverR(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorRecieverR(const SFunctorRecieverR& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    Return operator ()(void)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)();
    }

    Return call(void)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)();
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename T, typename A>
class SFunctorReciever1 : public SFunctor1<A>
{
protected:
    typedef void (T::*funcType)(A);
    funcType Callback;
    T* CallbackClass;
public:

    SFunctorReciever1(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorReciever1(const SFunctorReciever1& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    void operator ()(A var1)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1);
    }

    void call(A var1)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename Return, typename T, typename A>
class SFunctorRecieverR1 : public SFunctorR1<Return, A>
{
protected:
    typedef Return (T::*funcType)(A);
    funcType Callback;
    T* CallbackClass;
public:

    SFunctorRecieverR1(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorRecieverR1(const SFunctorRecieverR1& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    Return operator ()(A var1)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1);
    }

    Return call(A var1)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename T, typename A, typename B>
class SFunctorReciever2 : public SFunctor2<A, B>
{
protected:
    typedef void (T::*funcType)(A,B);
    funcType Callback;
    T* CallbackClass;
public:


    SFunctorReciever2(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorReciever2(const SFunctorReciever2& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    void operator ()(A var1, B var2)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1, var2);
    }

    void call(A var1, B var2)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1, var2);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename Return, typename T, typename A, typename B>
class SFunctorRecieverR2 : public SFunctorR2<Return, A, B>
{
protected:
    typedef Return (T::*funcType)(A, B);
    funcType Callback;
    T* CallbackClass;
public:


    SFunctorRecieverR2(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorRecieverR2(const SFunctorRecieverR2& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    Return operator ()(A var1, B var2)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1, var2);
    }

    Return call(A var1, B var2)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1, var2);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename T, typename A, typename B, typename C>
class SFunctorReciever3 : public SFunctor3<A, B, C>
{
protected:
    typedef void (T::*funcType)(A,B,C);
    funcType Callback;
    T* CallbackClass;
public:


    SFunctorReciever3(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorReciever3(const SFunctorReciever3& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    void operator ()(A var1, B var2, C var3)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1, var2, var3);
    }

    void call(A var1, B var2, C var3)
    {
        if (CallbackClass && Callback)
            (CallbackClass->*Callback)(var1, var2, var3);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

template<typename Return, typename T, typename A, typename B, typename C>
class SFunctorRecieverR3 : public SFunctorR3<Return, A, B, C>
{
protected:
    typedef Return (T::*funcType)(A, B, C);
    funcType Callback;
    T* CallbackClass;
public:


    SFunctorRecieverR3(T* callclass, funcType f)
    {
        Callback = f;
        CallbackClass = callclass;
    }

    SFunctorRecieverR3(const SFunctorRecieverR3& rec)
    {
        Callback = rec.Callback;
        CallbackClass = rec.CallbackClass;
    }

    Return operator ()(A var1, B var2, C var3)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1, var2, var3);
    }

    Return call(A var1, B var2, C var3)
    {
        if (CallbackClass && Callback)
            return (CallbackClass->*Callback)(var1, var2, var3);
    }

    void* getClassPointer(void)
    {
        return (void*)CallbackClass;
    }
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SFunctionReciever : public SFunctor
{
protected:
    typedef void (*funcType)();
    funcType Callback;
public:


    SFunctionReciever(funcType f)
    {
        Callback = f;
    }

    SFunctionReciever(const SFunctionReciever& rec)
    {
        Callback = rec.Callback;
    }

    void operator ()(void)
    {
        if (Callback)
            (*Callback)();
    }

    void call(void)
    {
        if (Callback)
            (*Callback)();
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename Return>
class SFunctionRecieverR : public SFunctorR<Return>
{
protected:
    typedef Return (*funcType)();
    funcType Callback;
public:


    SFunctionRecieverR(funcType f)
    {
        Callback = f;
    }

    SFunctionRecieverR(const SFunctionRecieverR& rec)
    {
        Callback = rec.Callback;
    }

    Return operator ()(void)
    {
        if (Callback)
            return (*Callback)();
    }

    Return call(void)
    {
        if (Callback)
            return (*Callback)();
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename A>
class SFunctionReciever1 : public SFunctor1<A>
{
protected:
    typedef void (*funcType)(A);
    funcType Callback;
public:


    SFunctionReciever1(funcType f)
    {
        Callback = f;
    }

    SFunctionReciever1(const SFunctionReciever1& rec)
    {
        Callback = rec.Callback;
    }

    void operator ()(A var1)
    {
        if (Callback)
            (*Callback)(var1);
    }

    void call(A var1)
    {
        if (Callback)
            (*Callback)(var1);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename Return, typename A>
class SFunctionRecieverR1 : public SFunctorR1<Return, A>
{
protected:
    typedef Return (*funcType)(A);
    funcType Callback;
public:


    SFunctionRecieverR1(funcType f)
    {
        Callback = f;
    }

    SFunctionRecieverR1(const SFunctionRecieverR1& rec)
    {
        Callback = rec.Callback;
    }

    Return operator ()(A var1)
    {
        if (Callback)
            return (*Callback)(var1);
    }

    Return call(A var1)
    {
        if (Callback)
            return (*Callback)(var1);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename A, typename B>
class SFunctionReciever2 : public SFunctor2<A, B>
{
protected:
    typedef void (*funcType)(A,B);
    funcType Callback;
public:


    SFunctionReciever2(funcType f)
    {
        Callback = f;
    }

    SFunctionReciever2(const SFunctionReciever2& rec)
    {
        Callback = rec.Callback;
    }

    void operator ()(A var1, B var2)
    {
        if (Callback)
            (*Callback)(var1, var2);
    }

    void call(A var1, B var2)
    {
        if (Callback)
            (*Callback)(var1, var2);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename Return, typename A, typename B>
class SFunctionRecieverR2 : public SFunctorR2<Return, A, B>
{
protected:
    typedef Return (*funcType)(A, B);
    funcType Callback;
public:


    SFunctionRecieverR2(funcType f)
    {
        Callback = f;
    }

    SFunctionRecieverR2(const SFunctionRecieverR2& rec)
    {
        Callback = rec.Callback;
    }

    Return operator ()(A var1, B var2)
    {
        if (Callback)
            return (*Callback)(var1, var2);
    }

    Return call(A var1, B var2)
    {
        if (Callback)
            return (*Callback)(var1, var2);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename A, typename B, typename C>
class SFunctionReciever3 : public SFunctor3<A, B, C>
{
protected:
    typedef void (*funcType)(A,B,C);
    funcType Callback;
public:


    SFunctionReciever3(funcType f)
    {
        Callback = f;
    }

    SFunctionReciever3(const SFunctionReciever3& rec)
    {
        Callback = rec.Callback;
    }

    void operator ()(A var1, B var2, C var3)
    {
        if (Callback)
            (*Callback)(var1, var2, var3);
    }

    void call(A var1, B var2, C var3)
    {
        if (Callback)
            (*Callback)(var1, var2, var3);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

template<typename Return, typename A, typename B, typename C>
class SFunctionRecieverR3 : public SFunctorR3<Return, A, B, C>
{
protected:
    typedef Return (*funcType)(A, B, C);
    funcType Callback;
public:


    SFunctionRecieverR3(funcType f)
    {
        Callback = f;
    }

    SFunctionRecieverR3(const SFunctionRecieverR3& rec)
    {
        Callback = rec.Callback;
    }

    Return operator ()(A var1, B var2, C var3)
    {
        if (Callback)
            return (*Callback)(var1, var2, var3);
    }

    Return call(A var1, B var2, C var3)
    {
        if (Callback)
            return (*Callback)(var1, var2, var3);
    }

    void* getClassPointer(void)
    {
        return (void*)NULL;
    }
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class SFunctorContainer
        {
        public:
            virtual ~SFunctorContainer(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, void (T::*func)(void))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorReciever<T>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call();
            }
        protected:
            irr::core::array< SFunctor* > Callbacks;
        private:
        };

        template<typename Return>
        class SFunctorContainerR
        {
        public:
            virtual ~SFunctorContainerR(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, Return (T::*func)(void))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorRecieverR<Return, T>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call();
            }
        protected:
            irr::core::array< SFunctorR<Return>* > Callbacks;
        private:
        };

        template<typename A>
        class SFunctorContainer1
        {
        public:
            virtual ~SFunctorContainer1(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, void (T::*func)(A))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorReciever1<T, A>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1);
            }
        protected:
            irr::core::array< SFunctor1<A>* > Callbacks;
        private:
        };

        template<typename Return, typename A>
        class SFunctorContainerR1
        {
        public:
            virtual ~SFunctorContainerR1(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, Return (T::*func)(A))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorRecieverR1<Return, T, A>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1);
            }
        protected:
            irr::core::array< SFunctorR1<Return, A>* > Callbacks;
        private:
        };

        template<typename A, typename B>
        class SFunctorContainer2
        {
        public:
            virtual ~SFunctorContainer2(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, void (T::*func)(A, B))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorReciever2<T, A, B>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1, B var2)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1, var2);
            }
        protected:
            irr::core::array< SFunctor2<A, B>* > Callbacks;
        private:
        };

        template<typename Return, typename A, typename B>
        class SFunctorContainerR2
        {
        public:
            virtual ~SFunctorContainerR2(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, Return (T::*func)(A, B))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorRecieverR2<Return, T, A, B>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1, B var2)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1, var2);
            }
        protected:
            irr::core::array< SFunctorR2<Return, A, B>* > Callbacks;
        private:
        };

        template<typename A, typename B, typename C>
        class SFunctorContainer3
        {
        public:
            virtual ~SFunctorContainer3(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, void (T::*func)(A, B, C))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorReciever3<T, A, B, C>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1, B var2, C var3)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1, var2, var3);
            }
        protected:
            irr::core::array< SFunctor3<A, B, C>* > Callbacks;
        private:
        };

        template<typename Return, typename A, typename B, typename C>
        class SFunctorContainerR3
        {
        public:
            virtual ~SFunctorContainerR3(void)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->drop();
                Callbacks.clear();
            }

            template<typename T>
            void addCallback(T* receiver, Return (T::*func)(A, B, C))
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                        return;
                Callbacks.push_back(new SFunctorRecieverR3<Return, T, A, B, C>(receiver, func));
            }

            template<typename T>
            void removeCallback(T* receiver)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    if (Callbacks[i]->getClassPointer() == (void*)receiver)
                    {
                        Callbacks[i]->drop();
                        Callbacks.erase(i);
                        return;
                    }
            }

            void Call(A var1, B var2, C var3)
            {
                for (irr::u32 i=0;i<Callbacks.size();i++)
                    Callbacks[i]->call(var1, var2, var3);
            }
        protected:
            irr::core::array< SFunctorR3<Return, A, B, C>* > Callbacks;
        private:
        };
////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // FUNCTOR_H
