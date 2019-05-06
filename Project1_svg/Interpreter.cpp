#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
#include "SVG.h"
#include "SVG.cpp"

int main(int argc, char** argv){
    int csv_index = 0;
    int data_number=0;
    vector<map<string, vector<string> > > Csv_Database;
    vector<string> field;
    string unique_id;
    
    if(argc>1){ //파일이 1개 이상 입력 되었다면 
    
    ifstream csv1File(argv[1]); //첫 번째 파일에 대해 
    if(csv1File.is_open()){
        string csvline;
        stringstream ss;
        map<string, vector<string> > csv_data;
        map<string, vector<string> >::iterator csv_data_iter;
        
        getline(csv1File,csvline);      //첫 번째 줄 읽기  
        replace(csvline.begin(), csvline.end(), ',', ' ');
        ss.str(csvline);
        int check_for_key = 0;
        string temp_line;
        while( ss >> temp_line ){
            if(check_for_key == 0){
                field.push_back(temp_line); //field vector에 field 이름을 저장  
                unique_id = temp_line;  //첫 번째 field는 unique id이므로 unique_id에 저장 
                check_for_key = 1;
            }else if(check_for_key == 1){
                field.push_back(temp_line); 
                
            }
        }
        getline(csv1File,csvline); 
        while(getline(csv1File,csvline)){   //자료 읽기  
            replace(csvline.begin(), csvline.end(), ',', ' ');
            stringstream iss(csvline);
            int i=0;
            int check_for_key = 0;
            string storage_for_key;
            while( iss >> temp_line ){
            if(check_for_key == 0){
                csv_data.insert(pair<string, vector<string> >(temp_line, vector<string>()));    //unique id 부분은 map의 key로 저장 
                storage_for_key = temp_line;
                check_for_key = 1;
            }else if(check_for_key == 1){
                csv_data[storage_for_key].push_back(temp_line);    //나머지는 map의 value로 저장 
            }
            } 
        }
        Csv_Database.push_back(csv_data);   //csv파일 1개를 Csv_Database에 push_back 
    }
    
    for(int k=2; k<argc; k++){  //나머지 파일에 대해 자료 저장 
    ifstream csv2File(argv[k]);
    if(csv2File.is_open()){
        string csvline;
        map<string, vector<string> > csv_data;
        map<string, vector<string> >::iterator csv_data_iter;
        getline(csv2File,csvline);
        getline(csv2File,csvline);
        while(getline(csv2File,csvline)){
            replace(csvline.begin(), csvline.end(), ',', ' ');
            stringstream iss(csvline);
            int i=0;
            int check_for_key = 0;
            string storage_for_key;
            string temp_line;
            while( iss >> temp_line ){
            if(check_for_key == 0){
                csv_data.insert(pair<string, vector<string> >(temp_line, vector<string>()));
                storage_for_key = temp_line;
                check_for_key = 1;
            }else if(check_for_key == 1){
                csv_data[storage_for_key].push_back(temp_line);   
            }
            }
        }
        Csv_Database.push_back(csv_data);
    }
}
}
/*////////////////////////////////////////////////////////////////////////////////////////////*/
    string command;
    string storage_for_tag;
    SVG *root_svg = new SVG("html");    //root_svg로 html svg생성 
while(true){
    cin >> command;
    if(command == "append"){
        string tag_name;
        cin >> tag_name;
        storage_for_tag = tag_name;
        root_svg->append(tag_name);
    }
    else if(command == "select"){
        string tag_name;
        cin >> tag_name;
        storage_for_tag = tag_name;
        root_svg->select(tag_name);
    }
    else if(command == "selectAll"){
        string tag_name;
        cin >> tag_name;
        storage_for_tag = tag_name;
        root_svg->select(tag_name);
    }
    else if(command == "remove"){
        root_svg->remove();
    }
    else if(command == "end"){
        if(root_svg->get_if_selected()==1) break;
        else root_svg->end();
    }
    else if(command == "enter"){
        int enter_index;
        cin >> enter_index;
        root_svg->enter(Csv_Database[enter_index-1], storage_for_tag, enter_index, unique_id);
    }
    else if(command == "update"){
        int enter_index;
        cin >> enter_index;
        root_svg->update(Csv_Database[enter_index-1], storage_for_tag, enter_index, unique_id);
    }
    else if(command == "exit"){
        int enter_index;
        cin >> enter_index;
        root_svg->exit(Csv_Database[enter_index-1], storage_for_tag, unique_id);
    }
    else if(command == "cattr"){
        string attr_name;
        string attr_value;
        cin >> attr_name >> attr_value;
        root_svg->cattr(attr_name, attr_value);
    }
    else if(command == "tattr"){
        string x_multiplier;
        string y_multiplier;
        cin >> x_multiplier >> y_multiplier;
        root_svg->tattr(x_multiplier, y_multiplier);
    }
    else if(command == "dattr"){
        string linear_input;
        getline(cin, linear_input);
        string input_storage[5];
        string svg_attr_name, datum_field_name;
        float mul = 0.0 , add = 0.0;
        stringstream piece;
        string piece_str;
        int count = 0;
        piece.str(linear_input);
        while( piece >> piece_str){
            input_storage[count++] = piece_str;
        }
        if(count == 2){     //mul과 add가 없을 때  
            svg_attr_name = input_storage[0];
            datum_field_name = input_storage[1];
            root_svg -> dattr(Csv_Database, field, svg_attr_name, datum_field_name);
        }else if(count == 3){   //mul까지 있을 때  
            svg_attr_name = input_storage[0];
            datum_field_name = input_storage[1];
            mul = atof(input_storage[2].c_str());
            add = 0.0;
            root_svg -> dattr(Csv_Database, field, svg_attr_name, datum_field_name, mul, add);
        }else if(count == 4){   //mul과 add 모두 있을 때 
            svg_attr_name = input_storage[0];
            datum_field_name = input_storage[1];
            mul = atof(input_storage[2].c_str());
            add = atof(input_storage[3].c_str());
            root_svg -> dattr(Csv_Database, field, svg_attr_name, datum_field_name, mul, add);
        }
    }
    else if(command == "print"){
        //root_svg -> print();
        string file_name;
        cin >> file_name;
        FILE *html_out;
        html_out = fopen(file_name.c_str(), "w");
        root_svg->print(html_out);
        fclose(html_out);
    }
}
    return 0;
}
