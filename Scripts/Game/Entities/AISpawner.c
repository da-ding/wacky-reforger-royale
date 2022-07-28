class AISpawnerEntityClass: DAD_SpawnRadiusEntityClass
{
	
}

class AISpawnerEntity: DAD_SpawnRadiusEntity 
{
	protected ref array<DAD_GroupSpawnPoint> m_SpawnPoints;
	protected ref array<ref SCR_UIInfo> Infos;
	
	private void CreateSpawns()
	{
	
	}

	override void OnActivate(IEntity ent)
	{
		Resource us   = Resource.Load("{63F01153A435D2BD}Prefabs/SpawnUS.et");
		Resource ussr = Resource.Load("{63F01153A435D2BD}Prefabs/SpawnUSSR.et");

		super.OnActivate(ent);
		foreach (int i, IEntity e : m_Entities)
		{
			auto group = SCR_AIGroup.Cast(e);

			string fac = group.m_faction;
			Resource res;
			if (fac == "US") {
				res = us;
			} else {
				res = ussr; 
			}
			
			DAD_GroupSpawnPoint spawn = DAD_GroupSpawnPoint.Cast(SpawnHelpers.SpawnEntity(res, group.GetOrigin()));
			LocalizedString s = fac + e + i.ToString();
			SCR_UIInfo info = SCR_UIInfo.CreateInfo(s)
			spawn.LinkInfo(info);
			Infos.Insert(info);

			spawn.SetGroup(group);
			spawn.ClearFlags(EntityFlags.STATIC, false);
			spawn.SetFactionKey(fac);

			spawn.Update();
			ent.Update();
		}
	}

};
