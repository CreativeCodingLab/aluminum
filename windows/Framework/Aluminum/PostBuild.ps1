# This script assumes we are starting in  ~/Dropbox/XCodeProjects/aluminum/windows/Framework/Aluminum
# Write-Host ""
# Get-Item Env:\NLTK_DATA # to use enviornmental variables...

# Write-Host $x
$BUILD="..\build"  # Our end result bundle
$SRC="..\..\..\include\Aluminum"    # aluminum source code
$WIN32="..\..\..\windows\include\Aluminum"    # windows source code
$DEBUG=".\Debug"    # location of our lib file

if (test-path $BUILD){
    rmdir -Recurse $BUILD
}

mkdir $BUILD\include\Aluminum, $BUILD\lib

# $CPP=@( ls $SRC_AL\*.cpp, $WIN32\*.cpp )
$HPP=@( ls $SRC\*.hpp, $WIN32\*.hpp, .\*.h )
$LIB="$DEBUG\Aluminum.lib"

foreach ($header in $HPP) {
    copy-item $header $BUILD\include\Aluminum.
}

copy-item $LIB $BUILD\lib\.