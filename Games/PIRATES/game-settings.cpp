


class Settings {

    public:

    Settings();  // Constructor
    ~Settings(); // Destructor

    void ReadSettings();
    int getNumbersFromString(std::string &s);
    // std::string removeAllWhitesAndBreakLinesInString(std::string& str);
    // std::string& removeCaracteresInString(std::string& input_string);
    // std::string replaceWordFromString(std::string subject, const std::string& search, const std::string& replace);
    // template<typename T> T removeWordsFromWideString(T SrcStart, T SrcEnd, const T TargetStart, const T TargetEnd);
    void Exit();

    private:

};

Settings::Settings(){};  // Constructor (Can accept arguments, but this one does not)
Settings::~Settings(){}; // Destructor (do not accept arguments)



void Settings::ReadSettings(){

    /// ---------------------------------------------------------------------------

    try {
    std::string line;
    std::ifstream inputfile;
    inputfile.open("settings.txt");

    while(!inputfile.eof()) {
    getline(inputfile, line);
    if(line.length()==0 || line[0]=='/'){

    /// Ignore empty or comment lines...

	} else {

    /// Parse this line...

    /// IF this line contains...
    if(line.find("FULLSCREEN")!=std::string::npos && line.find("true" )!=std::string::npos){ FULLSCREEN = true;  };
    if(line.find("FULLSCREEN")!=std::string::npos && line.find("false")!=std::string::npos){ FULLSCREEN = false; };

    /// IF this line contains...
    if(line.find("SCREEN_WIDTH")!=std::string::npos){ SCREEN_WIDTH = getNumbersFromString(line); };

    /// IF this line contains...
    if(line.find("SCREEN_HEIGHT")!=std::string::npos){ SCREEN_HEIGHT = getNumbersFromString(line); };

    /// IF this line contains...
    if(line.find("BACKGROUND_MUSIC")!=std::string::npos && line.find("true" )!=std::string::npos){ BACKGROUND_MUSIC = true;  };
    if(line.find("BACKGROUND_MUSIC")!=std::string::npos && line.find("false")!=std::string::npos){ BACKGROUND_MUSIC = false; };

    /// IF this line contains...
    if(line.find("VIEW_TEXTS")!=std::string::npos && line.find("true" )!=std::string::npos){ VIEW_TEXTS = true;  };
    if(line.find("VIEW_TEXTS")!=std::string::npos && line.find("false")!=std::string::npos){ VIEW_TEXTS = false; };

    /// IF this line contains...
    if(line.find("MAX_BALLS")!=std::string::npos){ MAX_BALLS = getNumbersFromString(line); };

    /// IF this line contains...
    if(line.find("NAMES_TEXTS")!=std::string::npos && line.find("true" )!=std::string::npos){ NAMES_TEXTS = true;  };
    if(line.find("NAMES_TEXTS")!=std::string::npos && line.find("false")!=std::string::npos){ NAMES_TEXTS = false; };

	};
    };

    if(inputfile){ inputfile.close(); };
    } catch (int e) { std::cout << e << std::endl; };

    /// ---------------------------------------------------------------------------

};





    /// ---------------------------------------------------------------------------------
    /// Small, Handy, Functions:
    /// ---------------------------------------------------------------------------------


    int Settings::getNumbersFromString(std::string &s) {
        std::stringstream ss(s);
        std::string tmp;
        char c;
        int value;
        ss >> tmp >> c >> value;
        return value;
    };



    /*
    std::vector<std::string> calling(){
        std::vector<std::string> file;
        std::string line;
        while(std::getline(std::cin, line)) { file.push_back(std::move(line)); };
        return file;
    };

    /// Filter these chars:
    bool my_filter1(char c){ return c==' ' || c=='=' || c==';' || c=='-' || c=='\n'; } // New code
    /// Function to remove above filtered chars:
    std::string& Settings::removeCaracteresInString(std::string& input_string){
    char * new_end;
    new_end=&*std::remove_if(input_string.begin(), input_string.end(), my_filter1);
    unsigned new_string_len = new_end - &*input_string.begin();
    input_string.resize(new_string_len);
    return input_string;
    };

    /// Function to removes all white spaces from a string:
    std::string Settings::removeAllWhitesAndBreakLinesInString(std::string& str){
    std::string temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] != ' ' && str[i] != '\n'){ temp += str[i]; };
    };
    return temp;
    };

    /// Function to replace word from string:
    std::string Settings::replaceWordFromString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
    };
    return subject;
    };

    /// Function to remove words from wide string: (for example the <code> tag)
    bool wordToRemoveFound = false;
    template<typename T> T Settings::removeWordsFromWideString(T SrcStart, T SrcEnd, const T TargetStart, const T TargetEnd){
    wordToRemoveFound = false;
    size_t TargetLen = TargetEnd-TargetStart;
    T i;
    while((i=std::search(SrcStart,SrcEnd,TargetStart,TargetEnd)) != SrcEnd){
    std::copy(i + TargetLen,SrcEnd+1,i);
    wordToRemoveFound = true;
    };
    return SrcStart;
    };

    // Convert INT to String:
    std::string Settings::convertIntToString(int number_in){
    stringstream ss;   // Create a stringstream
    ss << number_in;   // Add number to the stream
    return ss.str();   // Return a string with the contents of the stream
    };

    // Convert String to Upper Case:
    std::string Settings::convertToUpperInString(std::string string_in){
    for (size_t i = 0; i < string_in.length(); ++i){
    string_in[i]=toupper(string_in[i]);
    };
    return string_in.c_str();
    };

    // Convert String to Upper Case:
    std::wstring Settings::convertToUpperInWString(std::wstring string_in){
    for (size_t i = 0; i < string_in.length(); ++i){
    string_in[i]=toupper(string_in[i]);
    };
    return string_in.c_str();
    };

    // Filter these chars:
    bool my_filter2(char c){ return c==' ' || c==';' || c=='-' || c=='\n'; } // New code
    // Function to remove above filtered chars:
    wstring& removeCaracteresInWString(wstring& input_string){
    // char * new_end;
    wchar_t * new_end;
    new_end=&*std::remove_if(input_string.begin(), input_string.end(), my_filter2);
    unsigned new_string_len = new_end - &*input_string.begin();
    input_string.resize(new_string_len);
    return input_string;
    };

    // Function to remove the duplicates in an array:
    template<class Iterator>
    Iterator Settings::makeUnique(Iterator first, Iterator last){
    while (first != last){
    Iterator next(first);
    last = std::remove(++next, last, *first);
    first = next;
    };
    return last;
    };

    // Function to remove one white space from a string:
    std::string Settings::removeWhite(std::string& str){
    std::string temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] == ' ' && str[i-1] == ' '){} else { temp += str[i]; };
    };
    return temp;
    };

    // Function to removes all white spaces from a string:
    wstring Settings::removeAllWhitesAndBreakLinesInWstring(wstring& str){
    wstring temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] != L' ' && str[i] != L'\n'){ temp += str[i]; };
    };
    return temp;
    };

    // Function to split string into lines, using a delimiter: (for example the <br> tag)
    std::vector<std::string> splitThisString(const std::string &str, const std::string &delim){
    std::vector<std::string> ret;
    std::string::const_iterator prev = str.begin();
    for (std::string::const_iterator i = str.begin(); i < str.end() - delim.length()+1; ++i){
    if (equal(delim.begin(), delim.end(), i)){
    ret.push_back(std::string(prev,i));
    i += delim.length()-1;
    prev = i+1;
    };
    };
    ret.push_back(std::string(prev,str.end()));
    return ret;
    };

    // Function to split string into lines, using a delimiter: (for example the <br> tag)
    std::vector<wstring> splitThisWideString(const wstring &str, const wstring &delim){
    std::vector<wstring> ret;
    wstring::const_iterator prev = str.begin();
    for (wstring::const_iterator i = str.begin(); i < str.end() - delim.length()+1; ++i){
    if (equal(delim.begin(), delim.end(), i)){
    ret.push_back(wstring(prev,i));
    i += delim.length()-1;
    prev = i+1;
    };
    };
    ret.push_back(wstring(prev,str.end()));
    return ret;
    };

    // Round up a float type and show ONLY one decimal place:
    float Settings::roundUpThisFloat(float n){
    float t;
    t = n-floor(n);
    if(t>=0.5){n*=10; ceil(n); n/=10;
    } else {
    n*=10; floor(n); n/=10; };
    return n;
    };
    */


