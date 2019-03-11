/*main.cpp*/

//
//Nathan Jantz
//U. of Illinois, Chicago
//CS 341, Spring 2019
//Project #03: Grade Analysis Console App
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "gradeutil.h"

using namespace std;

/* Summary of College includes:
 * College Name
 * Number of Courses taught
 * Number of Students taught
 * Grade distribution (A-F)
 * DFW rate
 */
void Summary(const College& c){
  int x; //not used
  int N; //not used
  int numCourses = c.getNumCourses();
  int numStudents = c.getNumStudents();
  GradeStats temp = GetGradeDistribution(c);
  double DFW = GetDFWRate(c, x, N);
  //Output
  std::cout << c.Name << endl;
  std::cout << "# of courses taught: " << numCourses << endl;
  std::cout << "# of students taught: " << numStudents << endl;
  std::cout << "grade distribution (A-F): " << temp.PercentA
    << "%, " << temp.PercentB << "%, " << temp.PercentC << "%, "
    << temp.PercentD << "%, " << temp.PercentF << "%" << endl;
  std::cout << "DFW rate: " << DFW << "%" << endl << endl;
}

/* Summary of Department includes:
 * Department Name
 * Number of Courses taught
 * Number of Students taught
 * Grade distribution (A-F)
 * DFW rate
 */
void Summary(const Dept& d){
  int x; //not used
  int N; //not used
  int numCourses = d.Courses.size();
  int numStudents = d.getNumStudents();
  GradeStats temp = GetGradeDistribution(d);
  double DFW = GetDFWRate(d, x, N);
  //Output
  std::cout << d.Name << ":" << endl;
  std::cout << " # courses taught: " << numCourses << endl;
  std::cout << " # students taught: " << numStudents << endl;
  std::cout << " grade distribution (A-F): " << temp.PercentA
    << "%, " << temp.PercentB << "%, " << temp.PercentC << "%, "
    << temp.PercentD << "%, " << temp.PercentF << "%" << endl;
  std::cout << " DFW rate: " << DFW << "%" << endl;
}

/* Summary of Course includes:
 * Department, Number, (Section): Instructor
 * Number of Students taught
 * Course Type
 * Grade distribution (A-F)
 * DFW rate
 */
void Summary(const Course& c){
  int x; //not used
  int N; //not used
  int numStudents = c.getNumStudents();
  GradeStats temp = GetGradeDistribution(c);
  double DFW = GetDFWRate(c, x, N);
  std::string gType = "unknown";
  
  if(c.getGradingType() == Course::Letter){
    gType = "letter";
  }
  else if(c.getGradingType() == Course::Satisfactory){
    gType = "satisfactory";
  }

  //Output
  std::cout << c.Dept << " " << c.Number << " (section "
    << c.Section << "): " << c.Instructor << endl;
  std::cout << " # students: " << numStudents << endl;
  std::cout << " course type: " << gType << endl;
  std::cout << " grade distribution (A-F): " << temp.PercentA
    << "%, " << temp.PercentB << "%, " << temp.PercentC << "%, "
    << temp.PercentD << "%, " << temp.PercentF << "%" << endl;
  std::cout << " DFW rate: " << DFW << "%" << endl;
}

/*Summary Helper function
 * Prompts user for specific dept or "all"
 * if all, then output gives summary of every department
 * if specific dept, then summary is given for only that dept
 * if department not found, then error message is produced
 */
void _summaryHelper(const College& c){
  //prompt user for input
  std::string userInput;
  std::cout << "dept name, or all? ";
  std::cin >> userInput;
  
  //outputs summary for every department
  if(userInput == "all"){
    for(const Dept& d: c.Depts){
      Summary(d);
    }
  }
  //searches for userInput in Depts vector
  else{
    auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
      if(d.Name == userInput){ 
        return true;
      }
      else{ 
        return false;
      }
    });
    //department found
    if(itr != c.Depts.end()){
      Summary(*(itr));
    }
    //not found
    else{
      std::cout << "**dept not found" << endl;
    }
  }
}


/* Search function
 * prompts user for specific dept or 'all'
 * then specific dept, prompts user for course # or prefix
 * output gives the following:
 * Dept, Number (section): Instructor
 * Number of students
 * Course type
 * grade distribution(A-F)
 * DFW Rate
 */
void Search(const College& c){
  //temp department with courses vector
  Dept temp;
  
  //prompt user for input
  std::string userInput;
  std::string instructorPrefix;
  int courseNum;
  std::cout << "dept name, or all? ";
  std::cin >> userInput;
  std::cout << "course # or instructor prefix? ";
  std::cin >> instructorPrefix;
  
  stringstream ss(instructorPrefix); //create stringstream object
  ss >> courseNum; //try to convert input to a course #
  
  //input not numeric
  if(ss.fail()){ 
    
    //for entire college
    if(userInput == "all"){
      temp.Courses = FindCourses(c, instructorPrefix);
      for(const Course& course: temp.Courses){
        Summary(course);
      }
      if(temp.Courses.size() == 0){
      	std::cout << "**none found" << endl;
      }
    }
    
    //specific dept
    else{
      //find department
      auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
        if(d.Name == userInput){ 
          return true;
        }
        else{ 
          return false;
        }
      });
      //department found
      if(itr != c.Depts.end()){
        temp.Courses = FindCourses(*(itr), instructorPrefix);
        for(const Course& course: temp.Courses){
          Summary(course);
        }
        if(temp.Courses.size() == 0){
      	  std::cout << "**none found" << endl;
        }
      }
      //not found
      else{
      	std::cout << "**dept not found" << endl;
      }
    }
  }
  
  //input is numeric
  else{ 
    
    //for entire college
    if(userInput == "all"){
      temp.Courses = FindCourses(c, courseNum);
      for(const Course& course: temp.Courses){
        Summary(course);
      }
      if(temp.Courses.size() == 0){
      	std::cout << "**none found" << endl;
      }
    }
    
    //specific dept
    else{
      //find department
      auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
        if(d.Name == userInput){ 
          return true;
        }
        else{ 
          return false;
        }
      });
      //department found
      if(itr != c.Depts.end()){
        temp.Courses = FindCourses(*(itr), courseNum);
        for(const Course& course: temp.Courses){
          Summary(course);
        }
		if(temp.Courses.size() == 0){
		  std::cout << "**none found" << endl;
		}
      }
      //not found
      else{
      	std::cout << "**dept not found" << endl;
      }
    }
  }
}

/*Unknown function
 * prompts user for dept name or 'all'
 * returns summary of all courses marked as course::unknown
 * Dept, Number (section): Instructor
 * Number of students
 * Course type
 * grade distribution(A-F)
 * DFW Rate
 */
void Unknown(const College& c){
  //prompt user input
  std::string userInput;
  std::cout << "dept name, or all? ";
  std::cin >> userInput;
  
//outputs summary for every department
  if(userInput == "all"){
    for(const Dept& d: c.Depts){
      for(const Course& course: d.Courses){
        if(course.getGradingType() == Course::Unknown){
          Summary(course);
        }
      }
    }
  }
  //searches for userInput in Depts vector
  else{
    auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
      if(d.Name == userInput){ 
        return true;
      }
      else{ 
        return false;
      }
    });
    //department found, find unknown courses
    std::vector<Course> unknown;
    if(itr != c.Depts.end()){
      for (const Course& course: itr->Courses){
        if(course.getGradingType() == Course::Unknown){
          unknown.push_back(course);
        }
      }
      if(unknown.size() == 0){
      	std::cout << "**none found" << endl;
      }
      else{
      	for(const Course& course: unknown){
      		Summary(course);
      	}
      }
    }
    //not found
  }  
}

/* DFW prompts user for dept and threshold
 * Then returns courses with a dfw at or above
 * that threshold in descending order.
 * If two or more courses have the same dfw
 * then they are returned in ascending order by
 * dept, number, and section respectively. 
 */
void Dfw(const College& c){
  //prompt user input
  std::string userInput;
  double userInput2;
  std::cout << "dept name, or all? ";
  std::cin >> userInput;
  std::cout << "dfw threshold? ";
  std::cin >> userInput2;
  
  //dummy variables
  int x;
  int y;
  
  //variables to check DFWrate
  double DFWrate;
  
  //temp dept to add courses to later
  std::vector<Course> temp;
  
  //outputs summary for every department
  if(userInput == "all"){
    for(const Dept& d: c.Depts){
      for(const Course& course: d.Courses){
        DFWrate = GetDFWRate(course, x, y);
        if((DFWrate - userInput2) > 0.00){
          temp.push_back(course);
        }
      }
    }
	  if(temp.size() == 0){
	  	std::cout << "**none found" << endl;
	  }
  }
  
  //searches for userInput in Depts vector
  else{
    auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
      if(d.Name == userInput){ 
        return true;
      }
      else{ 
        return false;
      }
    });
    //department found
    if(itr != c.Depts.end()){
      for (const Course& course: itr->Courses){
        DFWrate = GetDFWRate(course, x, y);
        if((DFWrate - userInput2) > 0.00){
          temp.push_back(course);
        }
      }
      if(temp.size() == 0){
  		  std::cout << "**none found" << endl;
      }
    }
    //not found
    else{
    	std::cout << "**dept not found" << endl;
    }
  }  
  
 if(temp.size() != 0){
	  //sort temp courses vector
	  std::sort(temp.begin(), temp.end(), [&](const Course& a, const Course& b){
		if((GetDFWRate(a,x,y) - GetDFWRate(b,x,y)) == 0.00){
		  if(a.Dept == b.Dept){
		    if(a.Number == b.Number){
		      if(a.Section < b.Section){
		        return true;
		      }
		      else{
		        return false;
		      }
		    }
		    else if(a.Number < b.Number){
		      return true;
		    }
		    else{
		      return false;
		    }
		  }
		  else if(a.Dept < b.Dept){
		    return true;
		  }
		  else{
		    return false;
		  }
		}
		else if((GetDFWRate(a,x,y) - GetDFWRate(b,x,y)) > 0.00){
		  return true;
		}
		else{
		  return false;
		}
	  });
	  for(const Course& course: temp){
		Summary(course);
	  }
  }
}

/* Letter A prompts user for dept and threshold
 * then returns courses with a PercentA equal to
 * or greater than that threshold in descending order.
 * If there are two or more courses with the same PercentA
 * value, then they are returned in ascending order by dept,
 * Number, and section respectively. 
 */
void LetterA(const College& c){
  //prompt user input
  std::string userInput;
  double userInput2;
  std::cout << "dept name, or all? ";
  std::cin >> userInput;
  std::cout << "letter A threshold? ";
  std::cin >> userInput2;
  
  //variables to check PercentA
  GradeStats gStat;
  
  //temp dept to add courses to later
  std::vector<Course> temp;
  
  //outputs summary for every department
  if(userInput == "all"){
    for(const Dept& d: c.Depts){
      for(const Course& course: d.Courses){
        gStat = GetGradeDistribution(course);
        if((gStat.PercentA - userInput2) > 0.00){
          temp.push_back(course);
        }
      }
    }
      if(temp.size() == 0){
      	std::cout << "**none found" << endl;
      }
  }
  
  //searches for userInput in Depts vector
  else{
    auto itr = find_if(c.Depts.begin(), c.Depts.end(), [&](const Dept& d){
      if(d.Name == userInput){ 
        return true;
      }
      else{ 
        return false;
      }
    });
    //department found
    if(itr != c.Depts.end()){
      for (const Course& course: itr->Courses){
        gStat = GetGradeDistribution(course);
        if((gStat.PercentA - userInput2) > 0.00){
          temp.push_back(course);
        }
      }
      if(temp.size() == 0){
  	    std::cout << "**none found" << endl;
  	  }
    }
    //not found
    else{
    	std::cout << "**dept not found" << endl;
    }
  }
    
  if(temp.size() != 0){
	  //sort temp courses vector
	  std::sort(temp.begin(), temp.end(), [&](const Course& a, const Course& b){
		if((GetGradeDistribution(a).PercentA - GetGradeDistribution(b).PercentA) == 0.00){
		  if(a.Dept == b.Dept){
		    if(a.Number == b.Number){
		      if(a.Section < b.Section){
		        return true;
		      }
		      else{
		        return false;
		      }
		    }
		    else if(a.Number < b.Number){
		      return true;
		    }
		    else{
		      return false;
		    }
		  }
		  else if(a.Dept < b.Dept){
		    return true;
		  }
		  else{
		    return false;
		  }
		}
		else if((GetGradeDistribution(a).PercentA - GetGradeDistribution(b).PercentA) > 0.00){
		  return true;
		}
		else{
		  return false;
		}
	  });
	  for(const Course& course: temp){
		Summary(course);
	  }
  }
}

/*------------------------Begin Main()------------------------*/

int main(){
  //output set to have 2 digits to the right of decimal
  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  
  //input file
	std::ifstream ifs;
	std::string filename;
	std::cin >>	filename;
  
	//open as read-only
  ifs.open(filename, std::ifstream::in);
  
  //input string to be parsed into course info
  std::string input;
  
  //vector to add created courses //to be sorted through later
  std::vector<Course> allCourses;
  
  //read in to create CollegeName and skip header
  std::string collegeName;
  if(ifs.good()){
  
  	std::getline(ifs, input, ',');
    collegeName = "** College of " + input;
    std::getline(ifs, input, ',');
    collegeName = collegeName + ", " + input;
    std::getline(ifs, input);
    collegeName = collegeName + " " + input + " **";
    
    std::getline(ifs, input);
  }
  
  //read in and parse all classes in csv file
  while(getline(ifs, input)){
    allCourses.push_back(ParseCourse(input));
  }
  
  //sort allCourses
  std::sort(allCourses.begin(), allCourses.end(), [&](const Course& c, const Course& d){
    if(c.Dept == d.Dept){
      if(c.Number == d.Number){
        if(c.Section < d.Section){
          return true;
        }
      }
      else if(c.Number < d.Number){
        return true;
      }
      else{
        return false;
      }
    }
    else if(c.Dept < d.Dept){
      return true;
    }
    else{
      return false;
    }
  });
  
  auto itr = allCourses.begin();
  College college(collegeName);
  int counter = 0;
  while (itr != allCourses.end()){
    std::vector<Course> results (allCourses.size());
    std::string Dtemp = itr->Dept;
    auto i = std::copy_if(itr, allCourses.end(), results.begin(),
                          [&](const Course& c){
                            return (c.Dept == Dtemp);
                          });
    //shrink container to new size
    results.resize(std::distance(results.begin(), i));

    //create new dept and assign results to dept.Courses vector
    college.Depts.push_back(Dept(Dtemp));
    college.Depts[counter].Courses.assign(results.begin(), results.end());
    
    //advance itr to new location in sorted allCourses vector
    std::advance(itr, results.size());
    
    //advance counter for number of depts
    counter++;
  }
  //output college summary
  Summary(college);
  
  std::cout << "Enter a command> ";
  std::string command;
  std::cin >> command;
  
  while(command != "#"){
    if(command == "summary"){
      _summaryHelper(college);
    }
    else if(command == "search"){
      Search(college);
    }
    else if(command == "unknown"){
      Unknown(college);
    }
    else if(command == "dfw"){
      Dfw(college);
    }
    else if(command == "letterA"){
      LetterA(college);
    }
    else{
      std::cout << "**unknown command" << endl;
    }
    
    //get command from user
    std::cout << "Enter a command> ";
    std::cin >> command;
  }
  
  return 0;
}
