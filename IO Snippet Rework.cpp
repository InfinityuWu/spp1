static void write_bitmap(const std::filesystem::path& file_path, BitmapImage image){

	std::ofstream f(file_path, std::ios::out | std::ios::binary);
	if (!f.is_open()) throw std::exception{};
	
	const int padAmount = ((4 - (image.get_width() * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int infoHeaderSize = 40;
	const int fileSize = fileHeaderSize + infoHeaderSize + (image.get_width() * 3 + padAmount) * image.get_height();
	
	
	unsigned char fileHeader[fileHeaderSize];
	
	fileHeader[0] = 'B';	// bfType
	fileHeader[1] = 'M';

	for (int i = 0; i < 4; i++) {
		
		fileHeader[2+i] = fileSize >> (i * 8);	// bfSize
		
		infoHeader[4+i] = image.get_width() >> (i * 8);	// biWidth
		infoHeader[8+i] = image.get_height() >> (i * 8);	// biHeight
		
	}
	
	for (int i = 6; i < 14; i++) fileHeader[i] = (i != 10) ? 0 : (fileHeaderSize + infoHeaderSize);	// all 0, except bfOffBits


	unsigned char infoHeader[infoHeaderSize];
	
	for (int i = 0; i < 40; i++) infoHeader[i] = 0;
	
	infoHeader[0] = infoHeaderSize;	// biSize
	
	infoHeader[12] = 1;	// biPlanes
	
	infoHeader[14] = 24;	// biBitCount (color depth in bpp)
	
	
	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

	for (int y = 0; y < image.get_height(); y++) {
		
		for (int x = 0; x < image.get_width(); x++) {
			Pixel p = image.get_pixel(y, x);
			unsigned char color[] = {static_cast<unsigned char>(p.get_blue_channel()), static_cast<unsigned char>(p.get_green_channel()), static_cast<unsigned char>(p.get_red_channel())};
			f.write(reinterpret_cast<char*> (color), 3);
		}
		
		for (int i = 0; i < padAmount; i++) f.put(0);
		
	}
	
	f.close();

}