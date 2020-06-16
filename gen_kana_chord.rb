#This script generates the code
#kana_chord.c/register_kana_code()

indent = 1
$tabs = ""
indent.times do
	$tabs.concat("\t")
end
def indent_puts(str)
	print $tabs
	puts str
end

vowels = ["A", "I", "U", "E", "O", "YA", "YU", "YO"]
#ぁ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_" + v + ")) {"
	indent_puts "\tregister_kana(KANA_X" + v + ");"
	indent_puts "}"
end
vowels.push("YE")
#あ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_" + v + ")) {"
	indent_puts "\tregister_kana(KANA_" + v + ");"
	indent_puts "}"
end
#かさたなはまらがざだばぱ
consonants = ["K", "S", "T", "N", "H", "M", "R", "G", "Z", "D", "B", "P"]
for c in consonants
	for v in vowels
		indent_puts "else if(kana_chord == (BIT_CONSONANT_" + c + " | BIT_VOWEL_" + v + ")) {"
		indent_puts "\tregister_kana(KANA_" + c + v + ");"
		indent_puts "}"
	end
end
#わを
vowels = ["A", "O"]
for v in vowels
	indent_puts "else if(kana_chord == BIT_VOWEL_W" + v + ") {"
	indent_puts "\tregister_kana(KANA_W" + v + ");"
	indent_puts "}"
end
#うぁ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_WH" + v + ");"
	indent_puts "}"
end
vowels = ["I", "E"]
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_W" + v + ");"
	indent_puts "}"
end
#ゐゑ
for v in vowels
	indent_puts "else if(kana_chord == BIT_VOWEL_W" + v + ") {"
	indent_puts "\tregister_kana(KANA_WY" + v + ");"
	indent_puts "}"
end
#ゎ
indent_puts "else if(kana_chord == BIT_VOWEL_WY) {"
indent_puts "\tregister_kana(KANA_XWA);"
indent_puts "}"
#くぁ
#foreign sound
vowels = ["A", "I", "E", "O"]
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_Q" + v + ");"
	indent_puts "}"
end
indent_puts "else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WY)) {"
indent_puts "\tregister_kana(KANA_QWA);"
indent_puts "}"
#ぐぁ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_GW" + v + ");"
	indent_puts "}"
end
#すぃずぃ
consonants = ["S", "Z"]
for c in consonants
	indent_puts "else if(kana_chord == (BIT_CONSONANT_" + c + " | BIT_VOWEL_WI)) {"
	indent_puts "\tregister_kana(KANA_" + c + "WI);"
	indent_puts "}"
end
#つぁ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_TS" + v + ");"
	indent_puts "}"
end
#ふぁ
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_F" + v + ");"
	indent_puts "}"
end
indent_puts "else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WY)) {"
indent_puts "\tregister_kana(KANA_FYU);"
indent_puts "}"
#ゔぁ
vowels.push("U")
for v in vowels
	indent_puts "else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_W" + v + ")) {"
	indent_puts "\tregister_kana(KANA_V" + v + ");"
	indent_puts "}"
end
indent_puts "else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WY)) {"
indent_puts "\tregister_kana(KANA_VYU);"
indent_puts "}"
#てぃとぅてゅでぃどぅでゅ
consonants = ["T", "D"]
for c in consonants
	indent_puts "else if(kana_chord == (BIT_CONSONANT_" + c + "H | BIT_VOWEL_WI)) {"
	indent_puts "\tregister_kana(KANA_" + c + "HI);"
	indent_puts "}"
	indent_puts "else if(kana_chord == (BIT_CONSONANT_" + c + "H | BIT_VOWEL_WU)) {"
	indent_puts "\tregister_kana(KANA_" + c + "WU);"
	indent_puts "}"
	indent_puts "else if(kana_chord == (BIT_CONSONANT_" + c + "H | BIT_VOWEL_WY)) {"
	indent_puts "\tregister_kana(KANA_" + c + "HU);"
	indent_puts "}"
end
