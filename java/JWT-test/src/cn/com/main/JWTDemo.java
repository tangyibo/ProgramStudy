package cn.com.main;

import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.Claim;
import com.auth0.jwt.interfaces.DecodedJWT;

public class JWTDemo {

	public static final String SECRET = "fdfsaFSSDSA";
	public static final int calendarInterval = 10;

	public static String createToken(String uid, String user) {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("alg", "HS256");
		map.put("typ", "JWT");

		Date nowDate = new Date();
		Calendar nowTime = Calendar.getInstance();
		nowTime.add(Calendar.DATE, calendarInterval);
		Date expireDate = nowTime.getTime();

		try {		
			String token = JWT.create().withHeader(map) // header
					.withClaim("iss", "http://example.com") // payload
					.withClaim("aud", "http://test.org")
					.withClaim("uid", uid)
					.withClaim("sub", user)
					.withIssuedAt(nowDate) // sign time
					.withExpiresAt(expireDate) // expire time
					.sign(Algorithm.HMAC256(SECRET)); // signature

			return token;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	public static Map<String, Claim> verifyToken(String token) {
		DecodedJWT jwt = null;
		try {
			JWTVerifier verifier = JWT.require(Algorithm.HMAC256(SECRET)).build();
			jwt = verifier.verify(token);
			return jwt.getClaims();
		} catch (Exception e) {
			 //e.printStackTrace();
			// token 校验失败, 抛出Token验证非法异常
			return null;
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String token = JWTDemo.createToken("12", "test");
		if (null != token) {
			System.out.println(token);

			String new_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJBUFAiLCJpc3MiOiJTZXJ2aWNlIiwidXNlck5hbWUiOiJ0ZXN0IiwiZXhwIjoxNTMxNzM3NjkxLCJ1c2VySWQiOiIxMiIsImlhdCI6MTUzMDg3MzY5MX0.-x8146yUlx43UAvThf6RsomGjHmk_ho7u2GAmODyv78";
			Map<String, Claim> claims = JWTDemo.verifyToken(new_token);
			if (claims != null) {
				for (Map.Entry<String, Claim> entry : claims.entrySet()) {
					System.out.println(entry.toString());
				}
			}
			else {
				System.out.println("token verify failed!");
			}
		}

	}

}
