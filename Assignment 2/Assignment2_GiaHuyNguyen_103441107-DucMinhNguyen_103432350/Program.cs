using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AI_Assignment_2
{
    class Program
    {
        static void Main(string[] args)
        {
            string filepath = "test_HornKB.txt";
            
            

            KnowledgeBase kB = new KnowledgeBase(filepath);
            Clause query = kB.Query;
            
            TruthTable test = new TruthTable();
            ForwardChain fctest = new ForwardChain();

           
            if(test.solve(kB, query))
                Console.WriteLine("YES: " + test.Count);
            else
                Console.WriteLine("NO");
                    
        }
    }
}