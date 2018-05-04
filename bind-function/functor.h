#ifndef __MISC_FUNCTOR__
#define __MISC_FUNCTOR__

namespace misc {

    template<class ret_type, class arg_type>
    class function_base
    {
    public:
        function_base ( )        {}
        virtual ~function_base ( )        {}
        virtual ret_type operator() ( arg_type ) = 0;
        virtual ret_type operator() () = 0;
        virtual function_base<ret_type, arg_type>* clone ( ) const = 0;
    };

    template<class ret_type, class arg_type>
    class function_impl_normal : public function_base<ret_type, arg_type>
    {
    public:
        typedef ret_type ( * NORM_PROC ) ( arg_type );

        explicit function_impl_normal ( NORM_PROC proc ,arg_type arg=arg_type() ) : norm_proc_ ( proc ),arg_(arg)
        {
        }

        virtual ret_type operator() ( arg_type arg )
        {
            return norm_proc_ ( arg );
        }
        
        virtual ret_type operator() ( )
        {
            return norm_proc_ ( arg_ );
        }

        virtual function_impl_normal<ret_type, arg_type>* clone ( ) const
        {
            return new function_impl_normal<ret_type, arg_type>( norm_proc_,arg_ );
        }

    private:
        NORM_PROC norm_proc_;
        arg_type arg_;
    };

    template<class CS, class ret_type, class arg_type>
    class function_impl_class : public function_base<ret_type, arg_type>
    {
    public:
        typedef ret_type ( CS::* PROC )( arg_type );

        function_impl_class ( CS* obj, PROC proc, arg_type arg=arg_type() ) : obj_ ( obj ), proc_ ( proc ),arg_(arg)
        {
        }

        virtual ret_type operator() ( arg_type arg )
        {
            return (obj_->*proc_ )( arg );
        }
        
        virtual ret_type operator() ()
        {
            return (obj_->*proc_ )( arg_ );
        }

        virtual function_impl_class<CS, ret_type, arg_type>* clone ( ) const
        {
            return new function_impl_class<CS, ret_type, arg_type>( obj_, proc_ ,arg_);
        }

    private:
        CS* obj_;
        PROC proc_;
        arg_type arg_;
    };

    template<class ret_type, class arg_type>
    class function
    {
    public:
        typedef ret_type ( * NORM_PROC ) ( arg_type );

        explicit function ( function_base<ret_type, arg_type>* fun )
        : fun_ ( fun )
        {
        }

        explicit function ( NORM_PROC proc = 0,arg_type arg=arg_type() )
        : fun_ ( new function_impl_normal<ret_type, arg_type>( proc ) )
        {
        }

        ret_type operator() ( arg_type arg )
        {
            return fun_->operator() ( arg );
        }

        ret_type operator() ( )
        {
            return fun_->operator() ( );
        }
        
        ~function ( )
        {
            delete fun_;
        }

        function ( const function& fun )
        {
            fun_ = fun.fun_->clone ( );
        }

        void operator= ( const function& fun )
        {
            if ( this == &fun ) return;

            fun_ = fun.fun_->clone ( );
        }

    private:
        function_base<ret_type, arg_type>* fun_;
    };
    
    template<class ret_type, class arg_type>
    function<ret_type, arg_type> bind ( ret_type ( * proc )( arg_type ),arg_type arg )
    {
        function_base<ret_type, arg_type>* pf = new function_impl_normal<ret_type, arg_type>( proc,arg );
        return function<ret_type, arg_type>( pf );
    }

    template<class CS, class ret_type, class arg_type>
    function<ret_type, arg_type> bind ( ret_type ( CS::* proc )( arg_type ), CS* pc )
    {
        function_base<ret_type, arg_type>* pf = new function_impl_class<CS, ret_type, arg_type>( pc, proc );
        return function<ret_type, arg_type>( pf );
    }
    
    template<class CS, class ret_type, class arg_type>
    function<ret_type, arg_type> bind ( ret_type ( CS::* proc )( arg_type ), CS* pc, arg_type arg )
    {
        function_base<ret_type, arg_type>* pf = new function_impl_class<CS, ret_type, arg_type>( pc, proc,arg );
        return function<ret_type, arg_type>( pf );
    }

} // end namespace misc

#endif