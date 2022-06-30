#include<bits/stdc++.h>
using namespace std;
using namespace chrono;
#define endl "\n"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

unordered_map<string,long long int> wordList;

////////////////////////////////////////////////////////////////////

int min(int x, int y, int z) { return min(min(x, y), z); }

////////////////////////////////////////////////////////////////////

int editDist(string str1, string str2, int m, int n)
//Using a bottom-up dynamic programming approach
{
    int dp[m + 1][n + 1];  //to store results of the sub problems
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j]= 1+ min(dp[i][j - 1],dp[i - 1][j], dp[i - 1][j - 1]);
        }
    }

    return dp[m][n];
}

////////////////////////////////////////////////////////////////////



string spellCorrect(string& word){

	//If word exists in dictionary return the word
    if(wordList.find(word)!=wordList.end()){
        return word;
    }

	//calculate edit distances for current word and words in dictionary
    map<int,vector<string>> distances;

    for(auto& word_freq:wordList){
        int ed=editDist(word_freq.first,word, word_freq.first.length(), word.length());
        distances[ed].push_back(word_freq.first);
    }

	//Take the words with lowest edit distance, these are assumed to be the nearest correct words.
    auto lowestDistance=distances.begin();
    pair<string,long long int> mostProbableWord={"",-1};

	//among the nearest correct words find the most proable word. The most probable word is the most frequently used word in english
    for(string& s:lowestDistance->second){
        long long int s_freq=wordList[s];
        if(s_freq>mostProbableWord.second){
            mostProbableWord={s,s_freq};
        }
    }

    return mostProbableWord.first;
}




int main(){
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);

    // Reading all words and hashing them
    fstream dictFile;
    fstream freqFile;
    dictFile.open("dict.txt",ios::in); //read
    freqFile.open("freq.txt",ios::in); //read

    if(dictFile.is_open() && freqFile.is_open()){
        string s1,s2;
        while(getline(dictFile,s1) && getline(freqFile,s2)){
            long long int currentFreq=stoll(s2);
            wordList[s1]=currentFreq;
        }
        dictFile.close();
        freqFile.close();
    }
    else{
        cout<<"Cannot find required files"<<endl;
        return 0;
    }


///////////////////////////////////////////////////////////////////


    // Reading input sentence
    string inputString;
    getline(cin,inputString);
    int inputLength=inputString.length();


//////////////////////////////////////////////////////////////////



    cout<<endl;
    cout<<"Corrected paragraph is: "<<endl;
    cout<<endl;


    auto start1 = high_resolution_clock::now();  //starting the timer

    // Collecting individual words and correcting
    string word="";
    for(int i=0;i<inputLength;i++){
        if(inputString[i]==' ' || inputString[i]==',' || inputString[i]=='.'){
            if(word!=""){
                string correctWord=spellCorrect(word);
                cout<<correctWord<<' ';
                word="";
            }
        }
        else{
            if(inputString[i]>='A' && inputString[i]<='Z') inputString[i]+=32;
            word.push_back(inputString[i]);
        }
    }
    string correctWord=spellCorrect(word);
    cout<<correctWord<<' ';


///////////////////////////////////////////////////////////////


    cout<<endl;
    cout<<endl;
    auto stop1 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop1 - start1);

    cout<<"Execution Time: "<<duration.count()/1000.0<<" milliseconds"<<endl;
    return 0;
}
