"""distutils.command.build_scripts

Implements the Distutils 'build_scripts' command."""

__revision__ = "$Id: build_scripts.py 83053 2010-07-22 12:50:05Z tarek.ziade $"

import os, re
from stat import ST_MODE
from distutils import sysconfig
from distutils.core import Command
from distutils.dep_util import newer
from distutils.util import convert_path, Mixin2to3
from distutils import log

# check if Python is called on the first line with this expression
first_line_re = re.compile('^#!.*python[0-9.]*([ \t].*)?$')

class build_scripts(Command):

    description = "\"build\" scripts (copy and fixup #! line)"

    user_options = [
        ('build-dir=', 'd', "directory to \"build\" (copy) to"),
        ('force', 'f', "forcibly build everything (ignore file timestamps"),
        ('executable=', 'e', "specify final destination interpreter path"),
        ]

    boolean_options = ['force']


    def initialize_options(self):
        self.build_dir = None
        self.scripts = None
        self.force = None
        self.executable = None
        self.outfiles = None

    def finalize_options(self):
        self.set_undefined_options('build',
                                   ('build_scripts', 'build_dir'),
                                   ('force', 'force'),
                                   ('executable', 'executable'))
        self.scripts = self.distribution.scripts

    def get_source_files(self):
        return self.scripts

    def run(self):
        if not self.scripts:
            return
        self.copy_scripts()


    def copy_scripts(self):
        """Copy each script listed in 'self.scripts'; if it's marked as a
        Python script in the Unix way (first line matches 'first_line_re',
        ie. starts with "\#!" and contains "python"), then adjust the first
        line to refer to the current Python interpreter as we copy.
        """
        self.mkpath(self.build_dir)
        outfiles = []
        updated_files = []
        for script in self.scripts:
            adjust = False
            script = convert_path(script)
            outfile = os.path.join(self.build_dir, os.path.basename(script))
            outfiles.append(outfile)

            if not self.force and not newer(script, outfile):
                log.debug("not copying %s (up-to-date)", script)
                continue

            # Always open the file, but ignore failures in dry-run mode --
            # that way, we'll get accurate feedback if we can read the
            # script.
            try:
                f = open(script, "r")
            except IOError:
                if not self.dry_run:
                    raise
                f = None
            else:
                first_line = f.readline()
                if not first_line:
                    self.warn("%s is an empty file (skipping)" % script)
                    continue

                match = first_line_re.match(first_line)
                if match:
                    adjust = True
                    post_interp = match.group(1) or ''

            if adjust:
                log.info("copying and adjusting %s -> %s", script,
                         self.build_dir)
                updated_files.append(outfile)
                if not self.dry_run:
                    outf = open(outfile, "w")
                    if not sysconfig.python_build:
                        outf.write("#!%s%s\n" %
                                   (self.executable,
                                    post_interp))
                    else:
                        outf.write("#!%s%s\n" %
                                   (os.path.join(
                            sysconfig.get_config_var("BINDIR"),
                           "python%s%s" % (sysconfig.get_config_var("VERSION"),
                                           sysconfig.get_config_var("EXE"))),
                                    post_interp))
                    outf.writelines(f.readlines())
                    outf.close()
                if f:
                    f.close()
            else:
                if f:
                    f.close()
                updated_files.append(outfile)
                self.copy_file(script, outfile)

        if os.name == 'posix':
            for file in outfiles:
                if self.dry_run:
                    log.info("changing mode of %s", file)
                else:
                    oldmode = os.stat(file)[ST_MODE] & 0o7777
                    newmode = (oldmode | 0o555) & 0o7777
                    if newmode != oldmode:
                        log.info("changing mode of %s from %o to %o",
                                 file, oldmode, newmode)
                        os.chmod(file, newmode)
        # XXX should we modify self.outfiles?
        return outfiles, updated_files

class build_scripts_2to3(build_scripts, Mixin2to3):

    def copy_scripts(self):
        outfiles, updated_files = build_scripts.copy_scripts(self)
        if not self.dry_run:
            self.run_2to3(updated_files)
        return outfiles, updated_files
