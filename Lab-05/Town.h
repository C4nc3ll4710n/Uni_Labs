#ifndef  Town_h
#define  Town_h

#include <string>

class Town{
    private:
        int number_of_citizens;
        std::string mayor_name;
        std::string country_name;
        std::string town_name;

    public:
        Town();
        
        Town(int num_cit, std::string mayor, std::string country, std::string town);
        
        Town(const Town &to_copy);
        
        void change_mayor(std::string new_mayor_name);
        
        void change_num_of_cit(int new_num);

        int howmanycit();

        std::string& whoismayor();

        std::string& whatcountry();

        std::string&
        whattown();
        
        void Town_info();

        void WriteXML(std::ostream& o);

        ~Town();
};


#endif //Town_h
