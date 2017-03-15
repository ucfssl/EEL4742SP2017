#! /usr/bin/python

print(	"#ifndef MORSECODE_H__\n"
		"#define MORSECODE_H__\n\n"
		"#define index(x)\t((x) - 'A')\n\n"
	)

morse_alphabet = [
		['A', ".-"],
		['B', "-..."],
		['C', "-.-."],
		['D', "--."],
		['E', "."],
		['F', "..-."],
		['G', "--."],
		['H', "...."],
		['I', ".."],
		['J', ".---"],
		['K', "-.-"],
		['L', ".-.."],
		['M', "--"],
		['O', "---"],
		['P', ".--."],
		['Q', "--.-"],
		['R', ".-."],
		['S', "..."],
		['T', "-"],
		['U', "..-"],
		['V', "...-"],
		['W', ".--"],
		['X', "-..-"],
		['Y', "-.--"],
		['Z', "--.."]
	]

for letter in morse_alphabet[:-1]:
	# morse rules state that one dash is three units
	# and one dot is one unit
	# the space in between elements of a letter is one unit
	# the space after a letter is three dots, but we already have one
	# extra from the last letter
	letter_size = 3 * letter[1].count('-') \
			+ letter[1].count('.') \
			+ len(letter[1]) \
			+ 2
	print("[ index('{0}') ] = {{{1}, ".format(letter[0], letter_size), end='')
	for character in letter[1][:-1]:
		print('1, 0, ' if character == '.' else '1, 1, 1, 0, ', end='')
	print('1},' if letter[1][-1] == '.' else '1, 1, 1},')

letter = morse_alphabet[-1]
letter_size = 3 * letter[1].count('-') \
		+ letter[1].count('.') \
		+ len(letter[1]) \
		+ 2
print("[ index('{0}') ] = {{{1}, ".format(letter[0], letter_size), end='')
for character in letter[1][:-1]:
	print('1, 0, ' if character == '.' else '1, 1, 1, 0, ', end='')
print('1}' if letter[1][-1] == '.' else '1, 1, 1}')

print(	"\n\n#undef index\n\n"
		"#endif\t/* MORSCODE_H__ */"
	)
