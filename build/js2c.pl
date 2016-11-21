#!/usr/bin/perl
use strict;

my $lisence = "/* Copyright (c) 2016 by Lain.js authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the \"Software\"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */\n";
my $header = "#ifndef LAINJS_JS_H\n#define LAINJS_JS_H\n";
my $start_variable = "  const char mainjs[] = {\n";
my $end_variable = "0 };\n";
my $foot = "#endif\n";

# 1. lainjs.js into lainjs_js.h

open(my $in, "<./src/js/lainjs.js") || die "Cannot open lainjs.js\n";
open(my $out, ">./src/lainjs_js.h") || die "Cannot open lainjs_js.h\n";

print $out $lisence;
print $out $header;
print $out $start_variable;

my $data = do { local $/; <$in> };
my @chars = split //, $data;

my $count = 0;
foreach (@chars) {
    $count++;
    my $char = ord($_);
    print $out "$char," ;
    if($count % 10 == 0){
        print $out "\n" ;
    }
}

print $out $end_variable;
print $out "\nconst int mainjs_length \= $count\;\n";

# 2. src/js/*.js into lainjs_js.h

opendir DIR, "./src/js" or die "cannot open dir : $!";
my @filenames = grep { $_ =~ /\.js$/ } readdir DIR;
closedir DIR;
foreach(@filenames) {
    my $name = $_;
    $name =~ s{\.[^.]+$}{};

    print $out "const char $name\_n [] = \"$name\";\n";
    print $out "const char $name\_s [] = \{\n";

    open(my $in, "<./src/js/$name.js") || die "Cannot open $name.js\n";
    my $data = do { local $/; <$in> };
    my @chars = split //, $data;

    my $count = 0;
    foreach (@chars) {
        $count++;
        my $char = ord($_);
        print $out "$char," ;
        if($count % 10 == 0){
            print $out "\n" ;
        }
    }

    print $out "0 \}\;\n";
    print $out "const int $name\_l = $count;\n";
}

my $native_struct = <<'END_NATIVE_STRUCT';
struct native_mod {
  const char* name;
  const char* source;
  const int length;
};

 __attribute__ ((used)) static struct native_mod natives[] = {
END_NATIVE_STRUCT

print $out $native_struct;

foreach(@filenames) {
    my $name = $_;
    $name =~ s{\.[^.]+$}{};
    print $out "\{ $name\_n, $name\_s, $name\_l \},\n";
}

print $out "\{ NULL, NULL, 0 \}\}\;\n";

# add footer
print $out $foot;
