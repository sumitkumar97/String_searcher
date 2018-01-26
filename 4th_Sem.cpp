#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std ;

class PATTERN ;

class STRING {
protected :
  char str[1000000] ;
  int length ;
public :
  void getstring() ;
  void putstring() ;
  void cal_length() ;
  int len() {    return length ;  }
  char* strin() {    return str ;  }
  void string_copier(char *) ;
  friend int string_searcher ( STRING , PATTERN , int , int ) ;
  friend void print_in_color ( int , STRING , PATTERN ) ;
} ;

void STRING::getstring() {
  fgets ( str , 1000000 , stdin ) ;
  cal_length() ;
}

void STRING::putstring(){
  puts ( str ) ;
}

void STRING::cal_length() {
  length = strlen(str) - 1 ;
}

void STRING::string_copier ( char * medium ) {
  strcpy ( str , medium ) ;
  cal_length() ;
}

class PATTERN : public STRING {
public :
  int longestproperprefixsuffix[100000] ;
  void longest_proper_comman_prefix_suffix_calculator () ;
  friend int string_searcher ( STRING , PATTERN , int , int ) ;
} ;

  
void PATTERN::longest_proper_comman_prefix_suffix_calculator () {
  longestproperprefixsuffix[0] = 0 ;
  int matched_prefix , matched_suffix ;
  for ( int seg_size = 1 ; seg_size < length ;  seg_size ++ ) {
    for( int startingofsuffix = 1 ; startingofsuffix <= seg_size ; startingofsuffix ++ ) {
      matched_suffix = startingofsuffix , matched_prefix = 0 ;
      while ( matched_suffix <= seg_size && str[matched_suffix] == str[matched_prefix] ) {
	matched_prefix++ ;
	matched_suffix++ ;
      }
      if ( matched_suffix == seg_size + 1 ) {
	longestproperprefixsuffix[seg_size] = matched_prefix ;
	break ;
      }
    }
  }
}

int starting[10000] , starting_count = 0 , biggerstring_printing_iterator ;

void print_in_color ( int starting , STRING biggerstring , PATTERN pattern ) {
  for ( int i = starting ; i < starting + pattern.length ; i ++ ){
    cout << "\033[1;31m"<<biggerstring.str[i]<<"\033[0m";
    biggerstring_printing_iterator ++ ;
  }
}

int string_searcher ( STRING biggerstring ,PATTERN pattern , int flag , int linesetter ) {
  int patternindex  = 0 , count = 0 ;
  static int linenumber = 1 ;
  if ( !linesetter )
    linenumber = 1 ;
  for ( int stringindex = 0 ; stringindex < biggerstring.length ; stringindex ++ ) {
    if ( biggerstring.str[stringindex] == pattern.str[patternindex] ) {
      if ( patternindex == pattern.length - 1 ) {
	if ( flag == 2 )
	  printf("At line number %6d ,    " , linenumber ) ;
	cout << "Match found at position x = " << stringindex - ( pattern.length - 1 ) + 1 <<"\n\n" ; //added 1 because real index starts from 0
	starting [starting_count] = stringindex - ( pattern.length - 1 ) ;
	starting_count ++ ;
	patternindex = pattern.longestproperprefixsuffix[pattern.length - 1] ; 
	count++ ;
      }
      else  patternindex ++ ;
    }
    else {
      if ( patternindex != 0 ) {
	patternindex = pattern.longestproperprefixsuffix[ patternindex - 1 ] ;
	stringindex -- ;
      }
    }
  }
  if ( flag == 1 ) {
    if ( count == 1 )
      cout<<"\n1 match was found\n" ;
    else
      printf("\nTotal %d matches were found\n",count) ;
  }
  linenumber ++ ;
  return count ;
}

int main()
{
  int choice , control = 1 ;
  STRING biggerstring ;
  PATTERN pattern ;
  char medium[1000000] ;
  while ( control ) {
  cout << "----------------- String Searcher ------------------\n" ;
  cout << " 1. Search within string provided through stdin\n" ;
  cout << " 2. Search within multiple files through file ptr\n" ;
  cout << " 3. Exit\n" ;
  cout << "\n Enter your choice : " ;
  scanf("%d", &choice ) ;
  if ( choice == 3 )
    return 0 ;
  
  pattern.getstring() ;
  
  cout << "Enter the substring ( pattern ) to be searched\n" ;
  pattern.getstring() ;
  
  cout << "Pattern entered :  " ;
  pattern.putstring() ;

  pattern.longest_proper_comman_prefix_suffix_calculator() ;
  
  if ( choice == 1 ) {
    printf("Enter the string within which the pattern is to be searched\n" ) ;
    biggerstring.getstring() ;
  
    printf("String entered :  ")  ;
    biggerstring.putstring() ;

    string_searcher ( biggerstring , pattern , 1 , 0 ) ;
  
  }
  else if ( choice == 2 ) {
    char filename[30] ;
    int ans , n ;
    cout << "Enter the number of files\n" ;
    cin >> n ;
    while ( n -- ) {
      cout << "Enter the name of the file : " ;
      cin >> filename ;
      FILE * pfile ;
      pfile = fopen ( filename , "r" ) ;
      if ( pfile == NULL ){
	cout<<"File doesn't exist\n\n\n" ;
      }
      else {
	ans = 0 ;
	int linesetter = 0 ;
	while( !feof(pfile)) {
	  fgets ( medium , 1000000 , pfile ) ;
	  if ( !feof(pfile)) {
	    biggerstring.string_copier( medium ) ;
	    int prev_ans = ans ;
	    ans += string_searcher ( biggerstring , pattern ,  2 , linesetter) ;
	    if ( ans > prev_ans ){
	      biggerstring_printing_iterator = 0 ;
	      for ( int i = 0 ; i < starting_count ; i ++ ){
		while ( biggerstring_printing_iterator != starting[i] ){
		  cout<<biggerstring.strin()[biggerstring_printing_iterator] ;
		  biggerstring_printing_iterator ++ ;
		}
		print_in_color ( starting[i] , biggerstring , pattern ) ;
	      }
	      while ( biggerstring_printing_iterator <= biggerstring.len() ){
		cout<<biggerstring.strin()[biggerstring_printing_iterator] ;
		biggerstring_printing_iterator ++ ;
	      }
	      starting_count = 0 ;

	      cout<<endl ;
	    }
	    linesetter ++ ;
	  }
	}
	if ( ans == 1 )
	  cout << "\n1 match was found.\n" ;
	else
	  cout << "\n" << ans <<" matches were found\n" ;      
	fclose(pfile) ;
      }
    }
  }
  else if ( choice == 3 ) control = 0 ;
  else
    cout<<"Enter Valid Choice\n" ;
  }
  return 0 ;
}


