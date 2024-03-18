# 优秀prompt



Prompt：识别给定新闻文章中提到的命名实体（人员、组织、地点）。请生成一个 JSON 字典，其中根据实体类型将命名实体分为三个不同的组。键是实体的类型，值是字符串列表。
昨天，法拉第未来首席执行官亚当森·艾默生和戴姆勒公司首席执行官迪特尔·蔡澈宣布计划在柏林建设新的超级工厂。该工厂将是法拉第未来和戴姆勒的合资企业，为两家公司生产电动汽车和电池组，为该地区创造数千个就业机会。艾默生和蔡澈表示，柏林的战略位置，加上熟练的劳动力和强大的基础设施，使其成为扩张的理想选择。新的超级工厂旨在满足欧洲对电动汽车日益增长的需求，并为可持续的未来做出贡献。大众汽车首席执行官赫伯特·迪斯对这一消息表示欢迎，他表示加强合作将有利于汽车行业向电动汽车的转型
英文版本：Identify the named entities (people, organizations, locations) mentioned in the given news article. Please generate a JSON dictionary that lists the named entities in three separate groups based on their entity types. The key is the type of entity and the value is a list of strings.
Yesterday, Adamson Emerson, the CEO of Faraday, and Dieter Zetsche, the CEO of Daimler AG, announced plans to build a new Gigafactory in Berlin. The facility will be a joint venture between Faraday and Daimler, producing electric vehicles and battery packs for both companies, creating thousands of job opportunities in the region. Emerson and Zetsche stated that the strategic location of Berlin, coupled with its skilled workforce and strong infrastructure, makes it an ideal choice for expansion. The new Gigafactory aims to meet the growing demand for electric vehicles in Europe and contribute to a sustainable future. Volkswagen CEO Herbert Diess welcomed the news, saying greater collaboration will, benefit the auto industry's transition to e-mobility.

Answer：

```python
{
    "people": ["Adamson Emerson", "Dieter Zetsche", "Herbert Diess"],
    "organizations": ["Faraday", "Daimler AG", "Volkswagen"],
    "locations": ["Berlin", "Europe"]
}
```

