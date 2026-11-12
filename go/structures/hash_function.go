package structures

type HashFunction interface {
	HHash(string, int) int
	Clone() HashFunction
}

type PolynomialHash struct{}
type CuckooHash1 struct{}
type CuckooHash2 struct{}

func (PolynomialHash) HHash(key string, mesto int) int {
	var hash uint32
	for i := 0; i < len(key); i++ {
		hash = hash*31 + uint32(key[i])
	}
	return int(hash % uint32(mesto))
}
func (PolynomialHash) Clone() HashFunction { return PolynomialHash{} }
func (CuckooHash1) HHash(key string, mesto int) int {
	hash := 0
	for i := 0; i < len(key); i++ {
		hash = (hash*31 + int(key[i])) % mesto
	}
	return hash
}
func (CuckooHash1) Clone() HashFunction { return CuckooHash1{} }
func (CuckooHash2) HHash(key string, mesto int) int {
	hash := 0
	for i := 0; i < len(key); i++ {
		b := int(key[i])
		hash = (hash*17 + b + b/mesto) % mesto
	}
	return hash
}
func (CuckooHash2) Clone() HashFunction { return CuckooHash2{} }
