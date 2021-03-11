#include <iostream>
#include <list>
#include <fstream>

int main() {


    std::list<int> values;
    int top = 50000000;
    for (int i = 0; i < top; i++)
    if(i % 2 == 0){
        values.push_back(3);
    }else{
        values.push_back(0);
    }
    std:: ofstream myfile;
    myfile.open("array-values.txt");

    // Iterate over the list and display numbers
    myfile << top << "\n";
    for (int val : values)
        myfile << val << " ";
    myfile.close();
    std::cout << "Run everything okay" <<  std::endl;
    return 0;
}