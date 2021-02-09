# Copyright ESIEE (2009) 
#
# m.couprie@esiee.fr
#
# This software is an image processing library whose purpose is to be
# used primarily for research and teaching.
#
# This software is governed by the CeCILL  license under French law and
# abiding by the rules of distribution of free software. You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info". 
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability. 
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or 
# data to be ensured and,  more generally, to use and operate it in the 
# same conditions as regards security. 
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
#

proc Dialog_Create {top title args} {
	global dialog
	if [winfo exists $top] {
          destroy $top
	}
	eval {toplevel $top} $args
	wm title $top $title
	return 1
}

proc Dialog_Create_OLD {top title args} {
# 
# Remplacé pour cause de bug : le message de prompt utilisé lors de 
# la première création reste à l'identique pour les créations suivantes
#
	global dialog
	if [winfo exists $top] {
		switch -- [wm state $top] {
			normal {
				# Raise a buried window
				raise $top
			}
			withdrawn -
			iconic {
				# Open and restore geometry
				wm deiconify $top
				catch {wm geometry $top $dialog(geo,$top)}
			}
		}
		return 0
	} else {
		eval {toplevel $top} $args
		wm title $top $title
		return 1
	}
}

proc Dialog_Wait {top varName {focus {}}} {
	upvar $varName var

	# Poke the variable if the user nukes the window
	bind $top <Destroy> [list set $varName cancel]

	# Grab focus for the dialog
	if {[string length $focus] == 0} {
		set focus $top
	}
	set old [focus -displayof $top]
	focus $focus
	catch {tkwait visibility $top}
	catch {grab $top}

	# Wait for the dialog to complete
	tkwait variable $varName
	catch {grab release $top}
	focus $old
}

proc Dialog_Dismiss {top} {
	global dialog
	# Save current size and position
	catch {
		# window may have been deleted
		set dialog(geo,$top) [wm geometry $top]
		wm withdraw $top
	}
}

proc Dialog_Prompt { string defaultval} {
        global prompt
        set prompt(result) $defaultval
	set f .prompt
	if [Dialog_Create $f "Prompt" -borderwidth 10] {
		message $f.msg -text $string -aspect 1000
		entry $f.entry -textvariable prompt(result)
		set b [frame $f.buttons]
		pack $f.msg $f.entry $f.buttons -side top -fill x
		pack $f.entry -pady 5
		button $b.ok -text OK -command {set prompt(ok) 1}
		button $b.cancel -text Cancel \
			-command {set prompt(ok) 0}
		pack $b.ok -side left
		pack $b.cancel -side right
		bind $f.entry <Return> {set prompt(ok) 1 ; break}
		bind $f.entry <Control-c> {set prompt(ok) 0 ; break}
	}
	set prompt(ok) 0
	Dialog_Wait $f prompt(ok) $f.entry
	Dialog_Dismiss $f
	if {$prompt(ok)} {
		return $prompt(result)
	} else {
		return {}
	}
}

proc Dialog_Prompt_OK_NOK { string } {
        global prompt
	set f .prompt
	if [Dialog_Create $f "Prompt" -borderwidth 10] {
		message $f.msg -text $string -aspect 1000
		set b [frame $f.buttons]
		pack $f.msg $f.buttons -side top -fill x
		button $b.ok -text OK -command {set prompt(ok) 1}
		button $b.cancel -text Cancel \
			-command {set prompt(ok) 0}
		pack $b.ok -side left
		pack $b.cancel -side right
	}
	set prompt(ok) 0
	Dialog_Wait $f prompt(ok)
	Dialog_Dismiss $f
	return $prompt(ok)
}
