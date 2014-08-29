

#!/bin/bash



SYSTEM_INCLUDE="/usr/local/include/Aluminum"
SYSTEM_LIB="/usr/local/lib"


  ### 1. UNINSTALL aluminum static lib and headers into system lib and system include dir 
  echo -e "\n\n\nUninstalling static library to $SYSTEM_LIB\n\n"

  sudo rm /usr/local/lib/aluminum.a

  echo -e "\n\n\nUninstalling headers to $SYSTEM_INCLUDE\n\n"
 
  sudo rm -rf /usr/local/include/Aluminum

