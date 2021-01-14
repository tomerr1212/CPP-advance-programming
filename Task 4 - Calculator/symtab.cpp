#include "symtab.h"
#include "scanner.h"

Attributes &Symbol_table::get(string name)
{
    if (sym_tab.find(name) != sym_tab.end())
    {
        
        return sym_tab.find(name)->second;
    }
    // else
    return sym_tab.find(name)->second;
}

void Symbol_table::set(string name, double nv)
{
   // key  exist ?
   if (sym_tab.find(name) != sym_tab.end())
   {
       //is Attribute name is const ?
       if(sym_tab.find(name) ->second.is_const)
       {
           cout<<"error: cannot change constant"<<endl;
           exit(1);
       }
       else
       {
           sym_tab.find(name)->second.value  = nv;
        
       }

   }
  
}

bool Symbol_table::is_declared(string name)
{
    return (sym_tab.find(name) != sym_tab.end());
    
}

void Symbol_table::declare(string name, double nv, bool ic)
{
    Attributes *it = new Attributes(nv,ic);
    if(is_declared(name))
    {
        set(name, nv);
    }
    else
    {
        sym_tab.insert({name, *it});

    }
    
}
