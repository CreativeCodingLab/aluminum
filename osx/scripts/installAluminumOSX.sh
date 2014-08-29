
#!/bin/bash


if [ $# -eq 0 ]
  then
    BASE_DIR=".." #assuming running from with $OSX_DIR
    
  else
    BASE_DIR=$1 #assuming its been passed in
fi


SYSTEM_INCLUDE="/usr/local/include/Aluminum"
SYSTEM_LIB="/usr/local/lib"


  ### 1. INSTALL aluminum static lib and headers into system lib and system include dir 
  echo -e "\n\n\nInstalling static library to $SYSTEM_LIB\n\n"

  sudo cp $BASE_DIR/build/lib/aluminum.a $SYSTEM_LIB
  
  echo -e "\n\n\nInstalling headers to $SYSTEM_INCLUDE\n\n"
  
  sudo mkdir -p $SYSTEM_INCLUDE
  sudo cp $BASE_DIR/build/include/Aluminum/* $SYSTEM_INCLUDE
