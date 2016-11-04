import java.util.*;

public class hanoi {
	public static void moveHanoi(int n, String from, String to, String via) {
		if (n == 0)
			return;

		moveHanoi(n - 1, from, via, to);
		System.out.println("Move " + n + " from " + from + " to " + to);
		moveHanoi(n - 1, via, to, from);
	}

	public static void main(String[] args) {
		int k;

		k = Integer.parseInt(args[0]);

		moveHanoi(k, "a", "b", "c");
	}
}
