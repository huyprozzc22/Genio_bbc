from flask import Flask
from flask_restful import Api, Resource

app = Flask(__name__)
api = Api(app)

ids = {
"0015146948HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Nguyen Minh Dang", 		"age": 28, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "10"},
"0015171659HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Nguyen Nhat Hung Thinh",	"age": 23, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "6"},
"0015189899HoChiMinhCity":{"message":"yes", "type":"StudentCard", "name":"Dinh Duy Khanh", 			"age": 25, "sex":"male", "class": "U17", "years": "2022 - 2023", "lesson": "3"}, 
"0015208117HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Le Quoc Huy", 				"age": 27, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "10"},
"0015190291HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Manchester United", 		"age": 30, "sex":"male", "class": "U16", "years": "2021 - 2022", "lesson": "finished"}, 
"0015145887HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Bui Duc Dung", 			"age": 29, "sex":"male", "class": "U18", "years": "2021 - 2022", "lesson": "finished"},
"0015164197HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Luong Xuan Truong", 		"age": 26, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "10"},
"0015188872HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Doan van Hau", 			"age": 25, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "10"},
"0015209627HoChiMinhCity":{"message":"yes", "type":"StudentCard","name":"Nguyen Cong Phuong", 		"age": 23, "sex":"male", "class": "U18", "years": "2022 - 2023", "lesson": "6"}, 

"0000000001HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"01"},
"0000000002HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"02"},
"0000000003HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"03"},
"0000000004HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"04"},
"0000000005HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"05"},
"0000000006HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"06"},
"0000000007HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"07"},
"0000000008HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"08"},
"0000000009HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"09"},
"0000000010HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"10"},
"0000000011HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"11"},
"0000000012HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"12"},
"0000000013HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"13"},
"0000000014HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"14"},
"0000000015HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"15"},
"0000000016HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"16"},
"0000000017HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"17"},
"0000000018HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"18"},
"0000000019HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"19"},
"0000000020HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"20"},
"0000000021HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"21"},
"0000000022HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"22"},
"0000000023HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"23"},
"0000000024HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"24"},
"0000000025HoChiMinhCity":{"message":"yes", "type":"Genio Smart device", "BoxID":"25"}}

class RFID(Resource):
	def get(self, id):
		return ids[id]

api.add_resource(RFID,"/rfid/<string:id>")

if __name__ == "__main__":
	app.run(host='192.168.41.165', port=5000)