# Copyright (c) 2011 Concurrent, Inc.

module_depends log hadoop jar
module_annotate run about "run Cascading.Multitool"
module_annotate_block run usage <<USAGE
Runs the Cascading.Multitool jar with the provided parameters.

Usage:
  [flags] parameter=argument [parameter=argument [...]]

Flags:
  -h|--help    - displays this message
  -v|--version - displays the version of the Cascading.Multitool jar
  -q|--quiet   - suppresses INFO messages from Cascading and Hadoop

Parameters:
  See \`help parameters\` for more information.
USAGE

mt_run ()
{
  if [ $# -eq 0 ]; then
    error "ERROR No arguments specified"
    [ -z "$mt_run_avoid_exit" ] && exit 1

  else
    $HADOOP_BIN jar $mt_jar_path $@ 2>&1 | while read line; do
      if echo $line | grep INFO > /dev/null; then
        info $line
      elif echo $line | grep WARN > /dev/null; then
        warn $line
      elif echo $line | grep ERROR > /dev/null; then
        error $line
      elif echo $line | grep Exception > /dev/null; then
        stacktrace $line
      elif [[ ! -z $mt_log_verbose ]]; then
        log $line
      fi
    done
  fi
}