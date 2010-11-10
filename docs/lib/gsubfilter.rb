# Custom implementation of :colorize_syntax for handling code with '<' characters.
class GSubFilter < Nanoc3::Filter
	identifier :gsubfilter
	type :text

	def run( content, params = {} )
		content.gsub( params[:pattern], params[:replacement] )
	end
end
