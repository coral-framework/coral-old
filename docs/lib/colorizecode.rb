# Custom implementation of :colorize_syntax for handling code with '<' characters.
class ColorizeCode < Nanoc3::Filter
	identifier :colorize_code
	type :text

	def run( content, params = {} )
		while content =~ %r{(<code lang="(\w+)">(.+?[^(</code>)])</code>)}m
			orginal, lang, code = $1, $2, $3
			content.gsub!( orginal, highlight( code, lang ) )
		end
		content
	end

	def highlight( code, language, params = {} )
		extras = ""
		configDir = File.dirname(__FILE__) + "/highlight"
		case language
			when "cpp"
				extras = "--config-file=#{configDir}/cpp.lang"
			when "csl"
				extras = "--config-file=#{configDir}/csl.lang"
		end
		IO.popen( "highlight -S #{language} --replace-tabs=4 -O xhtml -f --enclose-pre #{extras}", "r+" ) do |io|
			io.write( code )
			io.close_write
			"<div class=\"sourcecode #{language}\">" + io.read + "</div>"
		end
	end
end
