object hello {
	class Math {
		def power(expr: Int, extent: Int): Int = {
			var result = 1
			for (i <- 1 to extent) {
				result *= expr
			}
			return result
		}
		def factorial(expr: Int): Int = {
			var result = 1
			for (i <- 1 to expr) {
				result *= i
			}
			return result
		}
	}

	class Complex(expr1: Int, expr2: Int) {
		private val real = expr1
		private val image = expr2

	}
	def main(args: Array[String]) {
		val math = new Math
		val complex = new Complex(1, 2)
		println(math.power(10, 3))
		println(math.factorial(5))
		println(complex))
	}
}