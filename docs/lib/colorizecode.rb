# Custom implementation of :colorize_syntax for handling code with '<' characters.
class ColorizeCode < Nanoc3::Filter
	identifier :colorize_code
	type :text

	def run( content, params = {} )
		content.gsub( %r{<code lang="(\w+)">(.+?[^(</code>)])</code>}m ) { |m|
			highlight( $1, $2 )
		}
	end

	def highlight( language, code, params = {} )
		extras = ""
		configDir = File.dirname(__FILE__) + "/highlight"
		case language
			when "cpp"
				extras = "--config-file=#{configDir}/cpp.lang"
			when "csl"
				extras = "--config-file=#{configDir}/csl.lang"
			when "lua"
				extras = "--config-file=#{configDir}/lua.lang"
		end
		IO.popen( "highlight -S #{language} --replace-tabs=4 -O xhtml -f --enclose-pre #{extras}", "r+" ) do |io|
			io.write( code )
			io.close_write
			"<div class=\"sourcecode #{language}\">" + io.read + "</div>"
		end
	end
end
