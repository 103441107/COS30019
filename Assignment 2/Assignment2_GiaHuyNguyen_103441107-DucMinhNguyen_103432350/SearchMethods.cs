using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AI_Assignment_2
{
    public abstract class SearchMethods
    {

        public SearchMethods()
        { 
        
        }

        public abstract bool solve(KnowledgeBase kb, Clause query);

    }
}
