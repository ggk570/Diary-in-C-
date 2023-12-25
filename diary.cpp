#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
// You need to change this PATH to path as per your need
#define PATH "D:\\Projects\\project C++\\Fun\\Diary\\"
using namespace std;

class Page{
    private:
        string date;
        string content;
        string fileName;
    public:
        Page(string d){
            date = d;
            fileName = PATH+date;
        }
        Page(int n){
            time_t timeAtN = time(0) + n * 86400;
            tm* timeStruct = gmtime(&timeAtN);
            timeStruct->tm_mon += 1;
            timeStruct->tm_year += 1900;
            date = to_string(timeStruct->tm_mday) +  "-" + to_string(timeStruct->tm_mon) + "-" + to_string(timeStruct->tm_year);
            fileName = PATH+date;
        }
        string getDate(){
            return date;
        }
        string getContent(){
            ifstream ipContent(fileName);
            if(!ipContent.is_open()) return "Error opening the file";
            string line;
            while(!ipContent.eof()){
                getline(ipContent, line);
                content += line + "\n";
            }
            return content;
        }
        int setContent(){
            ofstream ofContent(fileName);
            if(!ofContent.is_open()) return 0;
            cin.ignore();
            string line;
            while(getline(cin, line)){
                if(cin.eof()) break;
                ofContent<<line<<endl;
            }
            cin.clear();
            return 1;
        }
        void clearContent(){
            content = "";
        }
};

class Diary{
    private:
        int iterator;
        void deletePage(){
            delete currentPage;
        }
        bool validateDate(string date){
            regex datePattern(R"(([1-9]|[12][0-9]|3[01])-([1-9]|1[0-2])-(200[0-9]|201[0-9]|202[0-4]))");
            smatch match;
            if(!regex_match(date, match, datePattern)) return false;
            int day = stoi(match[1]);
            int month = stoi(match[2]);
            int year = stoi(match[3]);
            int daysInMonth[] = {31, 28 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

            if (day > daysInMonth[month - 1])
                return false;

            return true;
        }
    public:
        Page* currentPage;
        Diary(){
            currentPage = new Page(0);
            iterator = 0;
        }
        void gotoPreviousPage(){
            deletePage();
            currentPage = new Page(--iterator); 
        }
        void gotoNextPage(){
            deletePage();
            currentPage = new Page(++iterator);
        }
        void jumptoPage(){
            cout<<"You have to enter the date, ensure to put it in format d-m-yyyy"<<endl;
            cout<<"Enter the date where you want to go : ";
            string date;
            cin>>date;
            while(!validateDate(date)){
                cout<<"You are entering invalid format."<<endl;
                cout<<"Enter again : ";
                cin.ignore();
                cin.clear();
                cin>>date;
            }
            deletePage();
            currentPage = new Page(date);
        }
        string getCurrentPage(){
            return currentPage->getDate();
        }
};

void optionHandler(Diary* diary, int choice){
    string content = diary->currentPage->getContent();
    string date;
    switch(choice){
        case 1:
            if(content == "Error opening the file"){
                cout<<"There is some error reading the file"<<endl;
                break;
            }
            cout<<content;
            break;
        case 2:
            if(content != "" && content != "Error opening the file"){
                cout<<"You are trying to overwrite a page which is already written"<<endl;
                char yn;
                cout<<"Do you want to continue overwriting the file <y/n>? ";
                cin>>yn;
                if(yn == 'N' || yn == 'n') break;
                cout<<"Continuing with overwriting the file"<<endl;
            }
            cout<<"Enter details :"<<endl;
            if(diary->currentPage->setContent()) cout<<"Successfully written in file"<<endl;
            else cout<<"Failed to write due to some error"<<endl;
            break;
        case 3:
            diary->gotoPreviousPage();
            cout<<"Move to previous page"<<endl;
            break;
        case 4:
            diary->gotoNextPage();
            cout<<"Move to next page"<<endl;
            break;
        case 5:
            diary->jumptoPage();
            cout<<"Move to page for date : "<<diary->currentPage->getDate()<<endl;
            break;
        default:
            cout<<"Invalid Option Entered"<<endl;
            break;
    }
    diary->currentPage->clearContent();
}

void choiceLoop(Diary* diary){
    int option;
    while(true){
        cout<<"Current Page is For Date : "<<diary->getCurrentPage()<<endl;
        cout<<"Enter 0 to exit"<<endl;
        cout<<"Enter 1 to get content of current page"<<endl;
        cout<<"Enter 2 to set content of current page"<<endl;
        cout<<"Enter 3 to go to previous page"<<endl;
        cout<<"Enter 4 to go to next page"<<endl;
        cout<<"Enter 5 to go to custom page by giving date"<<endl;
        cout<<"Enter option : ";
        cin>>option;
        if(option == 0) exit(1);
        optionHandler(diary, option);
    }
}

int main(){
    Diary* d = new Diary();
    cout<<"Diary to save your daily happenings in text format"<<endl;
    choiceLoop(d);
    return 0;
}