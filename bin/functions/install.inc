# Copyright (c) 2011 Concurrent, Inc.

module_depends log
module_annotate install about "copy all files into place"
module_annotate_block install usage <<-USAGE
Copies all necessary files into place.

Usage:
  install [options]

Options:
  -d|--destination     - specifies the install destination
                         defaults to /usr/local/lib/multitool (as root/sudo)
                         defaults to $HOME/.multitool (as user)
  -b|--bin-destination - specifies the destination for executable files
                         defaults to /usr/local/bin
                         (only applies when running as root/sudo)
USAGE

mt_install_dirs=(lib bin)
mt_install_binfiles=(bin/mt)
mt_install_files=(README.txt LICENSE.txt *.jar)

if [ $UID -eq 0 ]; then
  mt_install_destination=/usr/local/lib/multitool
  mt_install_bin_destination=/usr/local/bin
else
  mt_install_destination=$HOME/.multitool
  mt_install_bin_destination=
fi

function mt_install () {
  mkdir -p $mt_install_destination

  for dir in "${mt_install_dirs[@]}" ; do
    info "INFO Installing $mt_install_destination/$dir"
    cp -Rf $MT_PATH/$dir $mt_install_destination
  done

  for file in "${mt_install_files[@]}" ; do
    info "INFO Installing $mt_install_destination/$file"
    cp -f $MT_PATH/$file $mt_install_destination
  done

  for binfile in "${mt_install_binfiles[@]}" ; do
    info "INFO Setting multitool base location to $mt_install_destination"
    cat $mt_install_destination/$binfile | sed s,MT_PATH=.,MT_PATH=\"$mt_install_destination\",g | tee $mt_install_destination/$binfile > /dev/null
  done

  if [[ $UID -eq 0 ]]; then
    mkdir -p $mt_install_bin_destination

    for binfile in "${mt_install_binfiles[@]}" ; do
      info "INFO Linking $mt_install_bin_destination/$binfile"
      chmod +x $mt_install_destination/$binfile
      ln -sf $mt_install_destination/$binfile $mt_install_bin_destination/`basename $binfile`
    done
  elif ! echo $PATH | grep "$mt_install_destination/bin" > /dev/null; then
    warn "WARN You should add $mt_install_destination/bin to your \$PATH."
  fi
}