# remove binaries

# Sourcefiles
GUI="fingerprint-gui"
SUID="fingerprint-suid"
IDENT="fingerprint-identifier"
HELPER="fingerprint-helper"
PLUGIN="fingerprint-plugin"
POLKIT="fingerprint-polkit-agent"
PAMSRC="fingerprint-pam/libpam_fingerprint-gui.so"

rm -f ./bin/$GUI/$GUI
rm -f ./bin/$SUID/$SUID
rm -f ./bin/$IDENT/$IDENT
rm -f ./bin/$HELPER/$HELPER
rm -f ./bin/$PLUGIN/$PLUGIN
rm -f ./bin/$POLKIT/$POLKIT
rm -f ./bin/$PAMSRC
