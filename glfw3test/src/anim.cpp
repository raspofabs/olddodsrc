float NewValue( float in ) {
	float i2 = in * in;
	float i3 = i2 * in;
	return -in;//- i2 * 0.004 + i3 * 0.000003;
	//return in - i2 * 0.004 + i3 * 0.000003;
}
