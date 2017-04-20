import java.math.BigDecimal;
import java.math.BigInteger;
import org.nd4j.linalg.util.BigDecimalMath;

public class RSA {
	
	public static BigInteger diff(BigInteger n, BigInteger mid) {
		BigInteger temp = mid.pow(3);
		if (n.compareTo(temp) == 1) 
			return (n.subtract(temp));
		else 
			return temp.subtract(n);
	}
	
	public static BigInteger cubicRoot(BigInteger n) {
		BigInteger start = BigInteger.ZERO;
		BigInteger end = n;
		BigInteger two = new BigInteger("2");
		BigInteger prevMid = BigInteger.ONE;
		while (true) {
			BigInteger mid = (start.add(end).divide(two));
			BigInteger error = 	diff(n, mid);
			
			if (error.compareTo(BigInteger.ZERO) == 0) {
				return mid;
			}
			if (prevMid.equals(mid)) {
				return BigInteger.ZERO;
			}
			if (mid.pow(3).compareTo(n) == 1){
				prevMid = mid;
				end = mid;
			}
			else{
				prevMid = mid;
				start = mid;
			}
				
		}
	}
	
	public static BigInteger kthRoot(BigInteger n, BigInteger k){
		BigInteger k1 = k.subtract(new BigInteger("1"));
		BigInteger s = n.add(new BigInteger("1"));
		BigInteger u = n;
		
		while (s.compareTo(u) == 1){
			s = u;
			BigInteger t = u.multiply(k1);
			u = t.add(n);
		}
		return s;
		
	}
	
//	public static BigInteger gcd(BigInteger a, BigInteger b){
//		if (a.equals(BigInteger.ZERO))
//			return b;
//		return gcd(a.mod(b),a);
//	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		
		BigInteger n1 = new BigInteger("7863362828396945422671641651092900868787418304416582734806554598466028883107969839732075710100920911073968048265197152545404817498266214859796653183913531");
		BigInteger n2 = new BigInteger("9076243440203680321542238609937774679337631521681187228501903278671607488481537902656962068758672732513386128438533711417528571581253925350724791101278249");
		BigInteger n3 = new BigInteger("9591484727325841676251343113176121253643898199338756148385302105171888523657925108317240391030375402041472558339579972248025672763160987601054264115379031");
		BigInteger c1 = new BigInteger("2766625776468490517020442165200622585353972364734257493634656285428849818088436679743017768288951285542149396220355412088478729843101404556595060419063530");
		BigInteger c2 = new BigInteger("3240126800603977412954894850356586499778223497401543135771704391187352751230185581072740440093233339171219744489365222351865844491714312596605562736155392");
		BigInteger c3 = new BigInteger("7826572050150696266209091757688995432053463211631140206664377915185639550299837873119352790719512120726002138492935624768097251665344346950673858876965204");
		
		assert ((n1.gcd(n2)).equals(BigInteger.ONE));
		assert ((n2.gcd(n3)).equals(BigInteger.ONE));
		assert ((n1.gcd(n3)).equals(BigInteger.ONE));
		
		System.out.println("CRT conditions are met. So we can apply CRT");
		
		BigInteger t1 = n2.multiply(n3);
		BigInteger t2 = n1.multiply(n3);
		BigInteger t3 = n1.multiply(n2);
		
		BigInteger B1 = t1.mod(n1);
		BigInteger B2 = t2.mod(n2);
		BigInteger B3 = t3.mod(n3);
//		
//		System.out.println(B1);
//		System.out.println(B2);
//		System.out.println(B3);
		
		BigInteger X1 = B1.modInverse(n1);
		BigInteger X2 = B2.modInverse(n2);
		BigInteger X3 = B3.modInverse(n3);

//		System.out.println(X1);
//		System.out.println(X2);
//		System.out.println(X3);
//		
		BigInteger B1X1 = t1.multiply(X1);
		BigInteger B2X2 = t2.multiply(X2);
		BigInteger B3X3 = t3.multiply(X3);
		
		BigInteger B1X1C1 = B1X1.multiply(c1);
		BigInteger B2X2C2 = B2X2.multiply(c2);
		BigInteger B3X3C3 = B3X3.multiply(c3);
		
		BigInteger s = B1X1C1.add(B2X2C2).add(B3X3C3);
		
		//verifying answer
		System.out.println("Verifying if M^e is correct or not");
		assert (s.mod(n1).compareTo(c1) == 0);
		assert (s.mod(n2).compareTo(c2) == 0);
		assert (s.mod(n3).compareTo(c3) == 0);
//		System.out.println(s.mod(n1));
//		System.out.println(s.mod(n2));
//		System.out.println(s.mod(n3));
		
		
		BigInteger N = n1.multiply(n2).multiply(n3);
		BigInteger minMess = s.mod(N);
		assert (minMess.mod(n1).compareTo(c1) == 0);
		assert (minMess.mod(n2).compareTo(c2) == 0);
		assert (minMess.mod(n3).compareTo(c3) == 0);

		System.out.println("Message raise to power 3 is = " + minMess);
		BigDecimal p = new BigDecimal(minMess);
		BigInteger message = cubicRoot(minMess);
		System.out.println("Taking cube root message is = "+ message);
		
		
		
//		System.out.println(message.pow(3));
//		BigInteger i = new BigInteger("13");
//		System.out.println(cubicRoot(minMess.add(N.multiply(i))));
	}

}
