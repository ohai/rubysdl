#!/usr/local/bin/ruby -Ke

class Hash
  def hash_map
    new_hash = Hash.new
    each{|k,v| new_hash[k] = yield k,v}
    new_hash
  end
end

def convert_link(str)
  (?a..?z).include?(str[0]) ? "((<SDL.#{str}>))" : "((<SDL::#{str}>))"
end

def convert_dlink(desc, link)
  (?a..?z).include?(link[0]) ? "((<#{desc}|SDL.#{link}>))" :
    "((<#{desc}|SDL::#{link}>))"
end

def inline(str)
  str.gsub(/\$\[(.*?)\]/,'((|\1|))').
    gsub(/\@\[(.*?)\|(.*?)\]/){ convert_dlink($1,$2) }.
    gsub(/\@\[(.*?)\]/){ convert_link($1) }
end

def format(lines,spaces)
  if lines
    lines.split(/^/).map{|line| " "*spaces + inline(line)}.join + "\n"
  else
    ""
  end
end

MethodDesc = Struct.new(:output, :purpose, :fullname, :module, :lock, :obsolete)

def rsd2rd(input)
  part = Hash.new{""}
  mode = nil
  
  input.each do |line|
    case line
    when /^(MOD|DEP|NAME|PURPOSE|TYPE|RVAL|OBSOLETE)\s+/
      part[$1] = $'.chomp
    when "LOCK\n"
      part["LOCK"] = ""
    when /^(PROTO|DESC|NOTES|RET|EXCEPTION|EXAMPLE|BUG|SEEALSO|COMMENT)\s*$/
      mode = $1
    when "EXCEPTION *\n"
      if $english
        part["EXCEPTION"] = "Raises @[Error] on failure"
      else
        part["EXCEPTION"] = "失敗したときには例外@[Error]を発生させます。\n"
      end
    else
      part[mode] += line
    end
  end

  %w(NAME PURPOSE TYPE PROTO).each do |v|
    raise "There isn't #{v} at #{part["NAME"]}" unless part.key?(v)
  end

  if !part.has_key?("DESC") && !part.has_key?("OBSOLETE")
    raise  "There isn't DESC and OBSOLETE at #{part["NAME"]}"
  end

  part = part.hash_map{|_, line| line.sub(/\n+\z/,"\n")}

  output = ""
  ns = if part.key?("MOD") then "SDL::#{part["MOD"]}" else "SDL" end

  part["PROTO"].each{|proto| output << "--- #{ns}#{part["TYPE"]}#{proto}"}
  output << "\n"
  if part.key?("OBSOLETE")
    if $english
      output << format("This method is obsolete. Please use @[#{part["OBSOLETE"]}] instead.\n", 4)
    else
      output << format("このメソッドの利用は推奨されません。代わりに@[#{part["OBSOLETE"]}]を利用してください。\n", 4)
    end
  end
  
  output << format(part["DESC"],4)
  
  if part.key?("LOCK")
    output << "\n"
    if $english
      output << format("This method needs @[Locking|Surface#lock].\nIf @[auto_lock?] is true, Ruby/SDL automatically locks/unlocks the surface.",4)
    else
      output << format("このメソッドを使うにはサーフェスを@[ロック|Surface#lock]する必要があります。\n@[auto_lock?]が真の場合はシステムが自動的にロック/アンロックします。",4)
    end
  end
  output << format(part["RET"],4)
  output << format(part["EXCEPTION"],4)
  if part.key?("DEP")
    if $english
      output << "\n    You need #{part["DEP"]} to use this method.\n"
    else
      output << "\n    このメソッドを使うには #{part["DEP"]} が必要です。\n"
    end
  end
  if part.key?("EXAMPLE")
    output << "\n    EXAMPLE\n"
    output << format(part["EXAMPLE"],6)
  end
  if part.key?("NOTES")
    output << "    * NOTES\n\n"
    output << format(part["NOTES"],6)
  end
  if part.key?("BUG")
    output << "    * BUG\n\n"
    output << format(part["NOTES"],6)
  end
  if part.key?("SEEALSO")
    output << "    * See Also\n      \n      "
    output << part["SEEALSO"].
      split(/\n/).
      map{|line| line[0] == ?( ? line : convert_link(line) }.
      join(", ")
    output << "\n\n"
  end

  MethodDesc.new(output,
                 part["PURPOSE"],
                 "#{ns}#{part["TYPE"]}#{part["NAME"]}",
                 part["MOD"],
                 part.key?("LOCK"),
                 part.has_key?("OBSOLETE"))
end

def toc(methods)
  methods.reject{|m| m.obsolete}.
    map{|m| "  * ((<#{m.fullname}>)) -- #{inline(m.purpose)}" }.join("\n")
end

def locklist(methods)
  methods.find_all{|m| m.lock}.map{|m| "* ((<#{m.fullname}>))"}.join("\n    ")
end

def methodlist(mod, methods)
  methods.find_all{|m| m.module == mod && !m.obsolete}.
    map{|m| "* ((<#{m.fullname}>)) -- #{inline(m.purpose)}"}.
    join("\n")
end

if ARGV.size == 1 && ARGV.shift == "-e"
  $english = true
else
  $english = false
end

synop, descs = ARGF.read.split(/^%%%$/)
methods = if descs then descs.split(/^%%$/).map{|m| rsd2rd(m)} else [] end

STDOUT << format(synop, 0).gsub(/^TOC$/){ toc(methods) }.
  gsub(/^METHODS\((.*)\)$/){methodlist($1, methods)}
  
methods.each{|m| STDOUT << m.output.gsub("LOCKLIST"){ locklist(methods) }}

