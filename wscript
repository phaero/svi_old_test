#! /usr/bin/env python
# encoding: utf-8

import os
import sys

import Options
import Utils

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='svi'

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def _run_cppcheck(bld):
	if bld.env['CPPCHECK']:
		print dir(bld)
		print dir(bld.get_targets())
		return
		for lib in bld.all_task_gen:
			for source_file in Utils.to_list(lib.source):
				cmd = '%s %s' % (
						bld.env['CPPCHECK'],
						os.path.join(lib.path.abspath(), source_file)
					)
				Utils.pproc.Popen(cmd, shell=True).wait()

def _cppcheck( task ):
	for source_file in task.inputs:
		cmd = '%s -v -a --enable=style %s' % ( task.env[ 'CPPCHECK' ], source_file.abspath() )
		task.exec_command( cmd )

def _run_astyle(bld):
	if bld.env['ASTYLE']:
		for lib in bld.all_task_gen:
			for source_file in Utils.to_list(lib.source):
				cmd = '%s --style=stroustrup --indent=tab -n -q %s' % (
						bld.env['ASTYLE'],
						os.path.join(lib.path.abspath(), source_file)
					)
				Utils.pproc.Popen(cmd, shell=True).wait()

def options(opt):
	opt.tool_options('compiler_c')

	opt.add_option('--cppcheck', default=False, dest='cppcheck', action='store_true')

def configure(conf):
	conf.check_tool('compiler_c')

	conf.check_cfg(package='glib-2.0', args='--cflags --libs', uselib_store='glib', mandatory=True)
	conf.find_program('cppcheck', var='CPPCHECK')
	conf.find_program('astyle', var='ASTYLE')

def build(bld):
	if Options.options.cppcheck:
		bld(
				rule=_cppcheck,
				source=bld.path.ant_glob( incl=('src/*.c', 'stest/*.c' ) ),
			)

	#bld.add_pre_fun(_run_astyle)

	bld.stlib(
			source = bld.path.ant_glob( 'stest/src/*.c' ),
			target = 'stest',
			uselib = 'glib',
			includes = 'stest/src stest/inc /usr/include',
			cflags = [ '-Wall', '-Wextra', '-pedantic', '-std=c99', '-g' ],
		)

	bld.program(
			source = bld.path.ant_glob( 'src/*.c', excl=['src/main.c',] ),
			target = '%s_ut' % APPNAME,
			use = 'stest',
			uselib = 'glib',
			includes = [ 'stest/inc', 'src', '/usr/include', ],
			cflags = [ '-Wall', '-Wextra', '-pedantic', '-std=c99', '-g', ],
		)

	bld.program(
			source = bld.path.ant_glob('src/*.c', excl=['src/*_test.c',]),
			target = APPNAME,
			uselib = 'glib',
			includes = 'src /usr/include',
			#ccflags = [ '-Wall', '-pedantic', '-std=c99', '-g', '-pg', ],
			cflags = [ '-Wall', '-Wextra', '-pedantic', '-std=c99', '-g', ],
			#linkflags = [ '-g', '-pg', ],
		)
