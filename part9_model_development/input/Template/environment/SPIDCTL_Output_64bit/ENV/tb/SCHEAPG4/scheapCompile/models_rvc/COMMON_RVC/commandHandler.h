//$Id: commandHandler.h,v 1.10 2012/04/06 03:07:13 watanama Exp $
// -----------------------------------------------------------------------------
// command I/F control model
// -----------------------------------------------------------------------------
#ifndef __commandHandler_h
#define __commandHandler_h

#include <stdarg.h>
#include <vector>
#include <map>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>

namespace vpcl
{


// -----------------------------------------------------------------------------
class command_processor_base
{
public:
  virtual std::string handleCommand(std::vector<std::string> &) = 0;

  virtual ~command_processor_base(){;}
};

// -----------------------------------------------------------------------------
template<class MODULE_TYPE, class FUNCPTR_TYPE>
class command_processor : public command_processor_base
{
public:
  command_processor(MODULE_TYPE* mod_name, FUNCPTR_TYPE handle_command_p)
  : command_processor_base()
  , mModule(mod_name), mFunc(handle_command_p) { ; }

  std::string handleCommand(std::vector<std::string> &args) {
    return (mModule->*mFunc)(args);
  }

private:
  MODULE_TYPE* mModule;
  FUNCPTR_TYPE mFunc;
};

// -----------------------------------------------------------------------------
class commandHandler {
public:
  //
  // initialize the load file stack
  //
  commandHandler(const char* fname = "") {
    assert(fname != NULL);

    mFileStack = new SFileInfo;
    assert(mFileStack != NULL);

    mFileStack->fname       = fname;
    mFileStack->num_of_line = 0;
    mFileStack->prev        = NULL;
    if (mFileStack->fname != "") {
      mFileStack->fin.open(fname);

      if (! mFileStack->fin.is_open()) {
        printf("Error (CommandHandler) Can't open \"%s\" file\n", fname);
      }
    }

    mEcho = false;
  };

  //
  // release the loaded file stack
  //
  ~commandHandler() {
    std::map<std::string, command_processor_base*>::iterator ite;
    for(ite=mModuleMap.begin(); ite!=mModuleMap.end(); ite++) {
      delete ite->second;
      ite->second = NULL;
    }

    while (mFileStack != NULL) {
      SFileInfo* prev = mFileStack->prev;
      delete mFileStack;
      mFileStack = prev;
    }
  };

  //
  // register instance and that api pointer
  //
  template<class MODULE_TYPE, class FUNCPTR_TYPE>
  void register_command_processor(std::string inst_name, MODULE_TYPE* mod_name, FUNCPTR_TYPE handle_command_p)
  {
    // over write check
    std::map<std::string, command_processor_base*>::iterator ite;
    for (ite=mModuleMap.begin() ; ite!=mModuleMap.end() ; ite++) {
      if (ite->first == inst_name) {
        printf("Error (CommandHandler) \"%s\" is registered yet\n", inst_name.c_str());
        return;
      }
    }
    mModuleMap[inst_name] = new command_processor<MODULE_TYPE, FUNCPTR_TYPE>(mod_name, handle_command_p);
    assert(mModuleMap[inst_name]!=NULL);
  }

  //
  // suspend to load the command and restart the simulation
  //
  void setContinue()
  {
    mContFlag = true;
  }

  //
  // command control I/F
  //
  void handleCommand()
  {
    mContFlag = false; // load the command until setContinue() is called

    std::string cur_line;

    while( getline((mFileStack->fname == "")? std::cin : mFileStack->fin, cur_line) ){
      mFileStack->num_of_line++;

      std::istringstream sin(cur_line);
      std::string command;
      std::string term;

      sin >> command;

      // dump specified command for debug
      if( mEcho && command != "" && command != "#" && command != "puts") {
        printf("# <%s>\n", cur_line.c_str());
      }

      if (command == "source") {
        std::string target;
        sin >> target;

        // check cyclic call
        bool is_error = false;
        for (SFileInfo* finfo_p = mFileStack ; finfo_p!=NULL ; finfo_p = finfo_p->prev) {
          if (target == finfo_p->fname) {
            printf("Error (CommandHandler) cyclic file called (%s:%d)\n"
            , mFileStack->fname.c_str(), mFileStack->num_of_line);
            is_error = true;
            break;
          }
        }
        if (is_error) {
          continue;
        }

        SFileInfo* info = new SFileInfo;
        assert(info != NULL);
        info->fname = target;
        info->num_of_line  = 0;

        info->fin.open(info->fname.c_str());
        if( ! info->fin.is_open() ) {
          printf("Error (CommandHandler) Can't open \"%s\" file (%s:%d)\n"
          , info->fname.c_str(), mFileStack->fname.c_str(), mFileStack->num_of_line);
          delete info;
        }
        else {
          info->prev = mFileStack;
          mFileStack = info;
          handleCommand();
          SFileInfo* tmp = mFileStack;
          mFileStack = mFileStack->prev;
          delete tmp;
        }
        //continue; // load next line
      }

      // comment check
      else if (command.substr(0,1) == "#" || command == "") {
        // do nothing
      }
      // command handler's command check
      else if (command == "help") {
        printf("    help                                      Show direction\n");
        printf("    go/run                                    (Re)start a simulation\n");
        printf("    quit/exit                                 Finish a simulation\n");
        printf("    ls [instance name]                        Display instance name\n");
        printf("    puts <message>                            Dump specified the message\n");
        printf("    <instance> <command> [argument]           Call instance's command\n");

      }
      else if (command == "go" || command == "run") {
        setContinue();
        break;
      }
      else if (command == "quit" || command == "exit") {
        exit(1);
      }
      else if (command == "echo") {
        std::string target;
        sin >> target;
        if (target == "on") {
          mEcho = true;
        }
        else if (target == "off") {
          mEcho = false;
        }
        else {
          printf("\"%s\" is invalid argument\n", target.c_str());
        }
      }
      else if (command == "ls") {
        bool each_check = false;
        bool matched = false;
        while( ! sin.eof() ){
          each_check = true;
          sin >> term;
          std::map<std::string, command_processor_base*>::iterator ite;
          for(ite=mModuleMap.begin(); ite!=mModuleMap.end(); ite++) {
            if (strmatch(term.c_str(), (ite->first).c_str())) {
              printf("%s\n", (ite->first).c_str());
              matched = true;
            }
          }
          if (!matched) {
            printf("\"%s\" not found\n", term.c_str());
          }
          matched = false;
        }

        if (!each_check) {
          std::map<std::string, command_processor_base*>::iterator ite;
          for(ite=mModuleMap.begin(); ite!=mModuleMap.end(); ite++) {
            printf("%s\n", (ite->first).c_str());
            matched = true;
          }
          if (!matched) {
            printf("Error (CommandHandler) Any model is not registered\n");
          }
        }
      }
      else if (command == "puts") {
        while( ! sin.eof() ){
          sin >> term;
          printf("%s ", term.c_str());
        }
        printf("\n");
      }

      else {
        // send a command to specified all instances
        // get argument
        std::vector<std::string> cmd;
        while( ! sin.eof() ){
          std::string target;
          sin >> target;
          if (target != "") {
            cmd.push_back(target);
          }
        }
        if( cmd.empty() ) {
          printf("Error (CommandHandler) command is not specified\n");
          continue;
        }

        bool is_inst_matched = false;
        bool is_cmd_found  = false;
        std::string rtn_val;
        std::map<std::string, command_processor_base*>::iterator it;
        for(it=mModuleMap.begin(); it!=mModuleMap.end(); it++) {
          if (strmatch(command.c_str(), (it->first).c_str())) {
            is_inst_matched = true;
            rtn_val = mModuleMap[it->first]->handleCommand(cmd);
            if (rtn_val.find("is invalid", 0) == std::string::npos) {
              is_cmd_found = true;
              if (rtn_val != "") {
                printf("%s\n", rtn_val.c_str());
              }
            }
          }
        }

        if( !is_inst_matched ) {
          printf("Error (CommandHandler) Unknown module name %s\n", command.c_str());
        }
        else if (!is_cmd_found) {
          printf("Error (CommandHandler) Unknown command name\n");
        }

        if( mContFlag == true ) break;  // continue
      }
    }
  }

private:
  std::map<std::string, command_processor_base*>  mModuleMap;

  bool mContFlag;        // a flag whether going on the simulation
  bool mEcho;            // echo back the command for debug

  struct SFileInfo {     // Stack the load file
    std::string   fname;
    unsigned int  num_of_line;
    std::ifstream fin;
    SFileInfo*    prev;
  };

  SFileInfo* mFileStack;

  //
  // wildcard expansion
  //
  bool strmatch(const char* ptn, const char* str) {
    bool ret = false;

    if (ptn == NULL || str == NULL) return false;

    switch( *ptn ) {
    case '\0':
      ret = '\0' == *str;
      break;
    case '*' :
      ret = strmatch( &ptn[1], str ) || (('\0' != *str) && strmatch( ptn, &str[1] ));
      break;
    case '?' :
      ret = ('\0' != *str) && strmatch( &ptn[1], &str[1] );
      break;
    default  :
      ret = ((unsigned char)*ptn == (unsigned char)*str) && strmatch( &ptn[1], &str[1] );
      break;
    }
    return ret;
  }
};
} // namespace vpcl
#endif
