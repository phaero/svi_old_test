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
		for lib in bld.all_task_gen:
			for source_file in Utils.to_list(lib.source):
				cmd = '%s %s' % (
						bld.env['CPPCHECK'],
						os.path.join(lib.path.abspath(), source_file)
					)
				Utils.pproc.Popen(cmd, shell=True).wait()

def _run_astyle(bld):
	if bld.env['ASTYLE']:
		for lib in bld.all_task_gen:
			for source_file in Utils.to_list(lib.source):
				cmd = '%s --style=stroustrup --indent=tab -n -q %s' % (
						bld.env['ASTYLE'],
						os.path.join(lib.path.abspath(), source_file)
					)
				Utils.pproc.Popen(cmd, shell=True).wait()

def set_options(opt):
	opt.tool_options('compiler_cc')

	opt.add_option('--cppcheck', default=False, dest='cppcheck', action='store_true')

def configure(conf):
	conf.check_tool('compiler_cc')

	conf.find_program('cppcheck', var='CPPCHECK')
	conf.find_program('astyle', var='ASTYLE')

def build(bld):
	if Options.options.cppcheck:
		bld.add_pre_fun(_run_cppcheck)

	bld.add_pre_fun(_run_astyle)

	bld.new_task_gen(
			features = 'cc cprogram',
			source = bld.path.ant_glob('src/*.c'),
			target = APPNAME,
			uselib = [],
			includes = './src /usr/include',
			#ccflags = [ '-Wall', '-pedantic', '-std=c99', '-g', '-pg', ],
			ccflags = [ '-Wall', '-pedantic', '-std=c99', '-g', ],
			#linkflags = [ '-g', '-pg', ],
		)

	bld.new_task_gen(
			features = 'cc cprogram',
			source = bld.path.ant_glob( 'src/**/*.c' ),
			target = '%s_ut' % APPNAME,
			uselib = [],
			includes = './src /usr/include',
			#ccflags = [ '-Wall', '-pedantic', '-std=c99', '-g', '-pg', ],
			ccflags = [ '-Wall', '-pedantic', '-std=c99', '-g', ],
			#linkflags = [ '-g', '-pg', ],
		)
