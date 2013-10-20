# Use this command after a compiler run with errors to remove auto-generated
# Makefiles. Then call "qmake" to create new ones.

find . | grep Makefile | grep -v ".mk" | grep -v "man" | xargs rm -f
